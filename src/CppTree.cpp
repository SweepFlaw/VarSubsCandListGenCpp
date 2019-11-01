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

CppTree::vSubsCandMap CppTree::getVarSubsCandMap(){
    return makeVarSubsCandMap(cursor);
}

CppTree::curTree CppTree::getCursorTree(){
    curTree ctree;

    struct cursorWithRange {
        CXCursor cur;
        unsigned begin;
        unsigned end;
    };

    struct ClData {
        string filename;
        vector<cursorWithRange> curVec;
    };
    ClData cldata {
        cppFilename,
        vector<cursorWithRange>()
    };

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

    for(size_t i = 0; i < cv.size(); ++i){
        cursorWithRange targetCursorRange = cv[i];
        cursorWithRange parentCursorRange = cv[0];

        bool findFlag = false;
        if(i > 0){
            for(size_t j = i - 1; j < cv.size(); --j){
                if(cv[j].end >= targetCursorRange.end && cv[j].begin <= targetCursorRange.begin){
                    findFlag = true;
                    parentCursorRange = cv[j];
                    break;
                }
            }
        }
        if(!findFlag){
            for(size_t j = i + 1; j < cv.size(); ++j){
                if(cv[j].begin <= targetCursorRange.begin && cv[j].end >= targetCursorRange.end){
                    findFlag = true;
                    parentCursorRange = cv[j];
                    break;
                }
            }
        }

        if(ctree.parent.count(targetCursorRange.cur) == 0){
            ctree.parent[targetCursorRange.cur] = parentCursorRange.cur;
            ctree.childs[parentCursorRange.cur].push_back(targetCursorRange.cur);
        }
    }

    return ctree;
}
