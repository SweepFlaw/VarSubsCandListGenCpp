#ifndef CPPSYNTH_CPPTREE_H
#define CPPSYNTH_CPPTREE_H

//#include <src/VarInfo.h>
#include <src/VarSubsCandMap.h>

#include <clang-c/Index.h>

class CppTree {
public:
    CXIndex cindex;
    CXTranslationUnit trunit;

    CppTree (char* cppFilename);
    ~CppTree();

    CXCursor getCursor();
    VarSubsCandMap getVarSubsCandMap();
};


#endif