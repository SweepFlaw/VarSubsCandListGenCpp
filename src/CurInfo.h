#ifndef CPPSYNTH_CURINFO_H
#define CPPSYNTH_CURINFO_H

#include <clang-c/Index.h>

#include <ostream>
#include <string>

class CurInfo {
public:
    CXCursor cursor;
    CXCursorKind ckind;
    CXSourceLocation location;
    CXType ctype;
    std::string curStr;
    bool isVariable;
    bool isInvalid;

    CXFile cfile;
    unsigned line;
    unsigned column;
    unsigned offset;
    
    CurInfo(CXCursor c);
    
    void printReadableStr(std::ostream& os);
};


#endif