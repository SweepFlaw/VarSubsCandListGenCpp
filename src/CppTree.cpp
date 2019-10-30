#include <src/CppTree.h>

#include <cstdlib>
#include <iostream>

using namespace std;

CppTree::CppTree(char* cppFilename)
{
    cindex = clang_createIndex(0,0);
    trunit = clang_parseTranslationUnit(
        cindex,
        cppFilename,
        nullptr, 0, nullptr, 0, CXTranslationUnit_None
    );
    if(trunit == nullptr) {
        cerr << "CppTree::CppTree - trunit is nullptr" << endl;
        exit(-1);
    }

    return;
}

CppTree::~CppTree()
{
    clang_disposeTranslationUnit(trunit);
    clang_disposeIndex(cindex);
}

CXCursor CppTree::getCursor()
{
    return clang_getTranslationUnitCursor(trunit);
}