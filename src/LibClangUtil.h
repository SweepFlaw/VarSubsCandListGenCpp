#ifndef CPPSYNTH_LIBCLANGUTIL_H
#define CPPSYNTH_LIBCLANGUTIL_H

#include <clang-c/Index.h>

#include <ostream>
#include <string>

class LibClangUtil{
public:
    static std::string getStr(CXString cxs, bool throwException=false);

    static std::string getCurKindStr(CXCursor c, bool throwException=false);
    static std::string getCurKindStr(CXCursorKind ck, bool throwException=false);
    static std::string getCurStr(CXCursor c, bool throwException=false);
    static std::string getFileStr(CXFile f, bool throwException=false);
    static std::string getTokenStr(CXTranslationUnit tru, CXToken t, bool throwException=false);
    static std::string getTypeStr(CXCursor c, bool throwException=false);
    static std::string getTypeStr(CXType c, bool throwException=false);

    static bool isCursorOnFile(CXCursor c, std::string filename, bool throwException=false);
    static bool isCursorScopeSeparatePoint(CXCursor c);
    static bool isCursorScopeSeparatePoint(CXCursorKind ck);
    static bool isCursorUeContent(CXCursor c);

    static CXSourceRange getCurRange(CXCursor c);

    static void printCursorValue(CXCursor c, std::ostream& os);

};



#endif