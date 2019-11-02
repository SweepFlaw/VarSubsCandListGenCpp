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
            if (lhs.begin < rhs.begin) return true;
            if (rhs.begin < lhs.begin) return false;

            if (lhs.end < rhs.end) return true;
            if (rhs.end < lhs.end) return false;
            return false;
        }
    );

    // figure out the parent-child relationships between cursors.
    for(size_t i = 0; i < cv.size(); ++i){
        cursorWithRange targetCursorRange = cv[i];
        cursorWithRange parentCursorRange = cv[0];

        bool findFlag = false;
        if(i > 0){
            for(size_t j = i - 1; j < cv.size(); --j){
                if((cv[j].end >= targetCursorRange.end) && (cv[j].begin <= targetCursorRange.begin)){
                    findFlag = true;
                    parentCursorRange = cv[j];
                    break;
                }
            }
        }
        if(!findFlag){
            for(size_t j = i + 1; j < cv.size(); ++j){
                if((cv[j].begin <= targetCursorRange.begin) && (cv[j].end >= targetCursorRange.end)){
                    findFlag = true;
                    parentCursorRange = cv[j];
                    break;
                }
            }
        }

        bool isDifferentCursor =
            (       std::hash<CXCursor>()(targetCursorRange.cur)
                !=  std::hash<CXCursor>()(parentCursorRange.cur));

        if(ctree.parent.count(targetCursorRange.cur) == 0 && findFlag && isDifferentCursor){
            ctree.parent[targetCursorRange.cur] = parentCursorRange.cur;
            ctree.childs[parentCursorRange.cur].push_back(targetCursorRange.cur);
        }
    }

    return ctree;
}

CppTree::varScopeMap CppTree::getVarScopeMap(curTree& ctree){
    struct ClData {
        curTree& ctr;
        varScopeMap vsm;
    };
    ClData cldata {
        ctree,
        varScopeMap()
    };

    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data) {
            ClData& cd = *(ClData*)client_data;
            CXCursorKind ck = clang_getCursorKind(c);
            CXCursor scopeDetCur;
            bool findFlag = false;
            if(ck == CXCursor_VarDecl){
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
            else if(ck == CXCursor_DeclRefExpr){
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