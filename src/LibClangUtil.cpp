#include <src/LibClangUtil.h>

#include <iostream>
#include <stdexcept>

using namespace std;

string LibClangUtil::getStr(CXString cxs, bool throwException){
    const char *cstr = clang_getCString(cxs);
    if (cstr == nullptr){
        if(throwException){
            throw invalid_argument("LibClangUtil::getStr - CStr is nullptr");
        } 
        else {return "";}
    }
    return string(cstr);
}

string LibClangUtil::getCurKindStr(CXCursor c, bool throwException){
    CXString cxs = clang_getCursorKindSpelling(clang_getCursorKind(c));
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

string LibClangUtil::getCurKindStr(CXCursorKind ck, bool throwException){
    CXString cxs = clang_getCursorKindSpelling(ck);
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

string LibClangUtil::getCurStr(CXCursor c, bool throwException){
    CXString cxs = clang_getCursorSpelling(c);
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

string LibClangUtil::getFileStr(CXFile f, bool throwException){
    CXString cxs = clang_getFileName(f);
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

string LibClangUtil::getTokenStr(CXTranslationUnit tru, CXToken t, bool throwException){
    CXString cxs = clang_getTokenSpelling(tru, t);
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

string LibClangUtil::getTypeStr(CXCursor c, bool throwException){
    CXString cxs = clang_getTypeSpelling(clang_getCursorType(c));
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

string LibClangUtil::getTypeStr(CXType ct, bool throwException){
    CXString cxs = clang_getTypeSpelling(ct);
    string s = getStr(cxs, throwException);
    clang_disposeString(cxs);
    return s;
}

bool LibClangUtil::isCursorOnFile(CXCursor c, string filename, bool throwException){
    // get the given cursor's file location
    CXSourceLocation loc = clang_getCursorLocation(c);
    CXFile cfile;
    unsigned lin, col, off;
    clang_getSpellingLocation(loc, &cfile, &lin, &col, &off);

    // compare the location and the given filename.
    string fns = getFileStr(cfile, throwException);
    return (fns.find(filename) != string::npos);
}

bool LibClangUtil::isCursorScopeSeparatePoint(CXCursor c){
    CXCursorKind ck = clang_getCursorKind(c);
    return (    ck == CXCursor_CompoundStmt
            ||  ck == CXCursor_DoStmt
            ||  ck == CXCursor_ForStmt
            ||  ck == CXCursor_IfStmt
            ||  ck == CXCursor_SwitchStmt
            ||  ck == CXCursor_WhileStmt
    );
}

bool LibClangUtil::isCursorScopeSeparatePoint(CXCursorKind ck){
    return (    ck == CXCursor_CompoundStmt
            ||  ck == CXCursor_DoStmt
            ||  ck == CXCursor_ForStmt
            ||  ck == CXCursor_IfStmt
            ||  ck == CXCursor_SwitchStmt
            ||  ck == CXCursor_WhileStmt
    );
}

bool LibClangUtil::isCursorUeContent(CXCursor c){
    CXCursorKind ck = clang_getCursorKind(c);
    return clang_isUnexposed(ck);
}

CXSourceRange LibClangUtil::getCurRange(CXCursor c){
    return clang_getCursorExtent(c);
}

void LibClangUtil::printCursorValue(CXCursor c, ostream& os){
    os  << "Cursor kind     :\t"    << c.kind   << endl
        << "Cursor xdata    :\t"    << c.xdata  << endl
        << "Cursor data     :\t"    << *(c.data)   << endl
    ;
}

