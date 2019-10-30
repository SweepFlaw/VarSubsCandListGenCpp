#ifndef CPPSYNTH_VARINFO_H
#define CPPSYNTH_VARINFO_H

#include <clang-c/Index.h>

class VarInfo {
public:
    char *varStr;
    char *typeStr;
    CXFile cfile;
    unsigned line;
    unsigned column;
    unsigned offset;
    CXSourceLocation location;
    
    VarInfo(CXCursor c);
    
};


#endif