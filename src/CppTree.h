#ifndef CPPSYNTH_CPPTREE_H
#define CPPSYNTH_CPPTREE_H

#include <src/misc.h>
//#include <src/VarInfo.h>
#include <src/VarSubsCandMap.h>

#include <clang-c/Index.h>

#include <unordered_map>
#include <string>
#include <utility>

class CppTree {
public:
    CXIndex cindex;
    CXTranslationUnit trunit;
    CXCursor cursor;
    std::string cppFilename;

    CppTree (const char* _cppFilename);
    ~CppTree();

    using curTree = 
        struct {
            std::unordered_map<CXCursor, std::vector<CXCursor>> childs;
            std::unordered_map<CXCursor, CXCursor> parent;
        };
    curTree getCursorTree();

    using varScopeMap = 
        struct {
            std::unordered_map<CXCursor, CXCursor> varDeclSM;
            std::unordered_map<CXCursor, CXCursor> varDRefSM;
        };
    varScopeMap getVarScopeMap(curTree& ctree);
    varScopeMap getVarScopeMap();

    //using varSubsCandMap = std::unordered_map<CXCursor, std::vector<CXCursor>>;
    //varSubsCandMap getVarSubsCandMap(curTree& ctree);
    //varSubsCandMap getVarSubsCandMap();
    
};


#endif