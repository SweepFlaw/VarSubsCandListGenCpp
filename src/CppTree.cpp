#include <src/CppTree.h>
#include <src/LibClangUtil.h>

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
    cppFilename = static_cast<string>(_cppFilename);

    return;
}

CppTree::~CppTree(){
    clang_disposeTranslationUnit(trunit);
    clang_disposeIndex(cindex);
}

CXCursor CppTree::getCursor(){
    return clang_getTranslationUnitCursor(trunit);
}

CppTree::vSubsCandMap CppTree::getVarSubsCandMap(){
    return makeVarSubsCandMap(getCursor());
}

CppTree::curTree CppTree::getCursorTree(){
    struct ClData {
        curTree ctree;
        string filename;
    };
    ClData cldata {
        curTree(),
        cppFilename
    };
    curTree ctree;
    clang_visitChildren(
        getCursor(),
        [](CXCursor c, CXCursor parent, CXClientData client_data) {
            ClData* cd = (ClData*)client_data;
            if(LibClangUtil::isCursorOnFile(c, cd->filename)){
                return CXChildVisit_Continue;
            }
            if(LibClangUtil::isCursorUeContent(c)){
                cd->ctree.insert({c, pair<CXCursor, vector<CXCursor>>(parent, vector<CXCursor>())});
                (cd->ctree)[parent].second.push_back(c);
            }
            return CXChildVisit_Recurse;
        },
        &cldata
    );

    return cldata.ctree;
}

static int debug_k = 1;
CppTree::curTree CppTree::getCursorTree_debug(){
struct ClData {
        curTree ctree;
        string filename;
    };
    ClData cldata {
        curTree(),
        cppFilename
    };
    curTree ctree;
    clang_visitChildren(
        getCursor(),
        [](CXCursor c, CXCursor parent, CXClientData client_data) {
            ClData* cd = (ClData*)client_data;
            if(LibClangUtil::isCursorOnFile(c, cd->filename)){
                return CXChildVisit_Continue;
            }
            if(LibClangUtil::isCursorUeContent(c)){
                cout << "iteration " << debug_k << endl;
                ++debug_k;
                cd->ctree.insert({c, pair<CXCursor, vector<CXCursor>>(parent, vector<CXCursor>())});
                (cd->ctree)[parent].second.push_back(c);
            }
            return CXChildVisit_Recurse;
        },
        &cldata
        );

    return cldata.ctree;




    //curTree ctree;
    //clang_visitChildren(
    //    getCursor(),
    //    [](CXCursor c, CXCursor parent, CXClientData client_data) {
    //        cout << "iteration " << debug_k << endl;
    //        ++debug_k;
    //        curTree* ct = (curTree*)client_data;
    //        ct->insert({c, pair<CXCursor, vector<CXCursor>>(parent, vector<CXCursor>())});
    //        (*ct)[parent].second.push_back(c);
    //        return CXChildVisit_Recurse;
    //    },
    //    &ctree
    //);

    //return ctree;

    //return ctree;
}