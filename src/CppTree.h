#ifndef CPPSYNTH_CPPTREE_H
#define CPPSYNTH_CPPTREE_H

#include <src/misc.h>

#include <clang-c/Index.h>

#include <unordered_map>
#include <string>
#include <utility>
#include <vector>

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
    // Construct cursorAST based on offset Range information.
    curTree getCursorTree();

    // varDeclSM is the ["var-decl-cursor" -> "scope-determine-cursor"] map 
    // varDRefSM is the ["var-refExpr-cursor" -> "scope-determine-cursor"] map
    using varScopeMap = 
        struct {
            std::unordered_map<CXCursor, CXCursor> varDeclSM;
            std::unordered_map<CXCursor, CXCursor> varDRefSM;
        };
    varScopeMap getVarScopeMap(curTree& ctree);
    varScopeMap getVarScopeMap();

    // varSubsCandMap is the ["var-refExpr-cursor" -> "available var-decl-cursors"] map
    // apply selectMode 0 for no filtering (only scope results are applied)
    // (selectMode % 10 == 1) for select the candidate which has exactly the same type (e.g. "const int" != "int")
    using varSubsCandMap = std::unordered_map<CXCursor, std::vector<CXCursor>>;
    varSubsCandMap getVarSubsCandMap(curTree& ctree, varScopeMap& vsmap, unsigned selectMode=0);
    varSubsCandMap getVarSubsCandMap(curTree& ctree, unsigned selectMode=0);
    varSubsCandMap getVarSubsCandMap(unsigned selectMode=0);
    
};


#endif