#ifndef CPPSYNTH_VARINFO_H
#define CPPSYNTH_VARINFO_H

#include <clang-c/Index.h>

#include <string>

class VarInfo {
public:
    CXCursor cursor;
    CXCursorKind ckind;
    CXSourceLocation location;
    CXType ctype;

    std::string varStr;
    CXFile cfile;
    unsigned line;
    unsigned column;
    unsigned offset;
    bool initFail;
    
    VarInfo(CXCursor c, bool throwException=false);
    
};


#endif