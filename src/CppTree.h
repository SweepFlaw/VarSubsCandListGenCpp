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
    std::string cppFilename;

    CppTree (const char* _cppFilename);
    ~CppTree();

    CXCursor getCursor();

    using vSubsCandMap = std::unordered_map<CXCursor, std::vector<CXCursor>>;
    vSubsCandMap getVarSubsCandMap();

    using curTree = 
        std::unordered_map<
            CXCursor,
            std::pair<CXCursor, std::vector<CXCursor>>,
            std::hash<CXCursor>,
            std::equal_to<CXCursor>
        >;

    curTree getCursorTree();
    curTree getCursorTree_debug();
};


#endif