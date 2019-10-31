#ifndef CPPSYNTH_VARINFO_H
#define CPPSYNTH_VARINFO_H

#include <clang-c/Index.h>

#include <ostream>
#include <string>

class VarInfo {
public:
    CXCursor cursor;
    CXCursorKind ckind;
    CXSourceLocation location;
    CXType ctype;
    std::string varStr;
    bool initFail;

    CXFile cfile;
    unsigned line;
    unsigned column;
    unsigned offset;
    
    VarInfo(CXCursor c, bool throwException=false);
    
    void printReadableStr(std::ostream& os);
};


#endif