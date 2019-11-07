#include <src/CppTree.h>
#include <src/CurInfo.h>
#include <src/LibClangUtil.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

CppTree::CppTree(const char* _cppFilename)
{
    cindex = clang_createIndex(0,0);
    trunit = clang_parseTranslationUnit(
        cindex,
        _cppFilename,
        nullptr, 0, nullptr, 0, CXTranslationUnit_None
    );
    if(trunit == nullptr) {
        cerr << "CppTree::CppTree - trunit is nullptr" << endl;
        exit(-1);
    }
    cursor = clang_getTranslationUnitCursor(trunit);
    cppFilename = static_cast<string>(_cppFilename);

    return;
}

CppTree::~CppTree(){
    clang_disposeTranslationUnit(trunit);
    clang_disposeIndex(cindex);
}

CppTree::curTree CppTree::getCursorTree(){
    curTree ctree;

    struct cursorWithRange {
        CXCursor cur;
        unsigned begin;
        unsigned end;
    };

    // client-data is the only way to pass the data or the function in clang_visitChildren.
    struct ClData {
        string filename;
        vector<cursorWithRange> curVec;
    };
    ClData cldata {
        cppFilename,
        vector<cursorWithRange>()
    };

    // Collect every cursors which points to source file and not unexposed contents.
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data) {
            ClData& cd = *(ClData*)client_data;

            if(!LibClangUtil::isCursorOnFile(c, cd.filename)){
                return CXChildVisit_Continue;
            }
            if(!LibClangUtil::isCursorUeContent(c)){
                CurInfo ci(c);
                unsigned begin = ci.range.begin_int_data;
                unsigned end = ci.range.end_int_data;
                if (begin > end){
                    begin = end;
                }
                cursorWithRange tmp;
                tmp.cur = c;
                tmp.begin = begin;
                tmp.end = end;
                cd.curVec.push_back(tmp);
            }

            return CXChildVisit_Recurse;
        },
        &cldata
    );

    // sort the vector by cursor's range-begin, range-end values.
    vector<cursorWithRange>& cv = cldata.curVec;
    std::sort(
        cv.begin(),
        cv.end(),
        [](const cursorWithRange& lhs, const cursorWithRange& rhs){
            return lhs.begin < rhs.begin;
        }
    );
    //std::sort(
    //    cv.begin(), 
    //    cv.end(), 
    //    [](const cursorWithRange& lhs, const cursorWithRange& rhs){
    //        if (lhs.begin < rhs.begin) return true;
    //        if (rhs.begin < lhs.begin) return false;
    //
    //        if (lhs.end < rhs.end) return true;
    //        if (rhs.end < lhs.end) return false;
    //        return false;
    //    }
    //);

    // figure out the parent-child relationships between cursors.
    for(size_t i = 0; i < cv.size(); ++i){
        cursorWithRange targetCursorRange = cv[i];
        cursorWithRange parentCursorRange;

        bool findFlag = false;
        for(size_t j = 0; j < cv.size(); ++j)
        {
            if(     i == j 
                ||  LibClangUtil::isTwoCursorEqual(targetCursorRange.cur, cv[j].cur)
                ||  cv[j].end < targetCursorRange.end
                ||  targetCursorRange.begin < cv[j].begin
            ) {
                if(targetCursorRange.begin < cv[j].begin){
                    break;
                }
                else{
                    continue;
                }
            }
            if(!(targetCursorRange.begin == cv[j].begin && targetCursorRange.end == cv[j].end )){
                if(!findFlag){
                    // first-find
                    parentCursorRange = cv[j];
                    findFlag = true;
                    continue;
                }
                // after second-find, find the cursor which has the shortest range.
                else if(     parentCursorRange.begin <= cv[j].begin
                    &&  cv[j].end <= parentCursorRange.end
                ) {
                    parentCursorRange = cv[j];
                }
            }
        }
        //if(i > 0){
        //    for(size_t j = i - 1; j < cv.size(); --j){
        //        if((cv[j].end >= targetCursorRange.end) && (cv[j].begin <= targetCursorRange.begin)){
        //            findFlag = true;
        //            parentCursorRange = cv[j];
        //            break;
        //        }
        //    }
        //}
        //if(!findFlag){
        //    for(size_t j = i + 1; j < cv.size(); ++j){
        //        if((cv[j].begin <= targetCursorRange.begin) && (cv[j].end >= targetCursorRange.end)){
        //            findFlag = true;
        //            parentCursorRange = cv[j];
        //            break;
        //        }
        //    }
        //}


        if(ctree.parent.count(targetCursorRange.cur) == 0){
            if(findFlag && !(LibClangUtil::isTwoCursorEqual(targetCursorRange.cur, parentCursorRange.cur))){
                ctree.parent[targetCursorRange.cur] = parentCursorRange.cur;
                ctree.childs[parentCursorRange.cur].push_back(targetCursorRange.cur);
            }
            else if(!LibClangUtil::isTwoCursorEqual(targetCursorRange.cur, cursor)){
                ctree.parent[targetCursorRange.cur] = cursor;
                ctree.childs[cursor].push_back(targetCursorRange.cur);
            }
        }
    }

    return ctree;
}

CppTree::varScopeMap CppTree::getVarScopeMap(curTree& ctree){
    struct ClData {
        string filename;
        curTree& ctr;
        varScopeMap vsm;
    };
    ClData cldata {
        cppFilename,
        ctree,
        varScopeMap()
    };

    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data) {
            ClData& cd = *(ClData*)client_data;
            if(!LibClangUtil::isCursorOnFile(c, cd.filename)){
                return CXChildVisit_Continue;
            }
            CXCursorKind ck = clang_getCursorKind(c);
            CXCursor scopeDetCur;
            bool findFlag = false;
            if(LibClangUtil::isCursorDecl(ck)){
                auto p = cd.ctr.parent.find(c);
                do {
                    if(p == cd.ctr.parent.end()){
                        break;
                    }
                    if(LibClangUtil::isCursorScopeSeparatePoint(p->second)){
                        scopeDetCur = p->second;
                        findFlag = true;
                        break;
                    }
                    p = cd.ctr.parent.find(p->second);
                } while(true);
                if(findFlag){
                    cd.vsm.varDeclSM[c] = scopeDetCur;
                }
            }
            else if(LibClangUtil::isCursorRefExpr(ck)){
                auto p = cd.ctr.parent.find(c);
                do{
                    if(p == cd.ctr.parent.end()){
                        break;
                    }
                    if(LibClangUtil::isCursorScopeSeparatePoint(p->second)){
                        scopeDetCur = p->second;
                        findFlag = true;
                        break;
                    }
                    p = cd.ctr.parent.find(p->second);
                } while (true);
                if(findFlag){
                    cd.vsm.varDRefSM[c] = scopeDetCur;
                }
            }

            return CXChildVisit_Recurse;
        },
        &cldata
    );

    return cldata.vsm;
}

CppTree::varScopeMap CppTree::getVarScopeMap(){
    auto ctree = getCursorTree();
    return getVarScopeMap(ctree);
}

// apply selectMode 0 for no filtering (only scope results are applied)
// (selectMode % 10 == 1) for select the candidate which has exactly the same type (e.g. "const int" != "int")
CppTree::varSubsCandMap CppTree::getVarSubsCandMap(curTree& ctree, varScopeMap& vsmap, unsigned selectMode){
    varSubsCandMap vscm;
    auto parInfo = ctree.parent;

    for(auto decl : vsmap.varDeclSM){
        auto declScopeCursor = decl.second;
        for(auto refe : vsmap.varDRefSM){

            // CASE (trimMode % 10 == 1)
            if(selectMode % 10 == 1){
                // if two variable types are different, skip it.
                auto decltyp = clang_getCursorType(decl.first);
                auto refetyp = clang_getCursorType(refe.first);
                if(!clang_equalTypes(decltyp, refetyp)){
                    continue;
                }
            }

            auto refScopeCursor = refe.second;
            if (LibClangUtil::isTwoCursorEqual(declScopeCursor, refScopeCursor)){
                if(CurInfo(declScopeCursor).range.begin_int_data <= CurInfo(refScopeCursor).range.begin_int_data){
                    vscm[refe.first].push_back(decl.first);
                    continue;
                }
            }

            auto rsc = refScopeCursor;
            while(true){
                auto p = ctree.parent.find(rsc);
                if(p == ctree.parent.end()) {
                    break;
                }
                else if (LibClangUtil::isTwoCursorEqual(declScopeCursor, p->second)){
                    if(CurInfo(declScopeCursor).range.begin_int_data <= CurInfo(refScopeCursor).range.begin_int_data){
                        vscm[refe.first].push_back(decl.first);
                        break;
                    }
                }
                rsc = p->second;
            }

            //auto p = ctree.parent.find(refScopeCursor);
            //do{
            //    if(p == ctree.parent.end()){
            //        break;
            //    }
            //    if(LibClangUtil::isTwoCursorEqual((p->second), declScopeCursor)){
            //        vscm[refe.first].push_back(decl.first);
            //    }
            //    p = ctree.parent.find(p->second);
            //} while(true);
        }
    }

    return vscm;
}

CppTree::varSubsCandMap CppTree::getVarSubsCandMap(curTree& ctree, unsigned selectMode){
    auto vsm = getVarScopeMap(ctree);
    return getVarSubsCandMap(ctree, vsm, selectMode);
}

CppTree::varSubsCandMap CppTree::getVarSubsCandMap(unsigned selectMode){
    auto ctree = getCursorTree();
    return getVarSubsCandMap(ctree, selectMode);
}