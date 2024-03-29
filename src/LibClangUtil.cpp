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
    return LibClangUtil::isCursorScopeSeparatePoint(ck);
}

bool LibClangUtil::isCursorScopeSeparatePoint(CXCursorKind ck){
    return (    
            // statements
            ck == CXCursor_CompoundStmt
            ||  ck == CXCursor_DoStmt
            ||  ck == CXCursor_ForStmt
            ||  ck == CXCursor_IfStmt
            ||  ck == CXCursor_SwitchStmt
            ||  ck == CXCursor_WhileStmt
            // RootNode
            ||  ck == CXCursor_TranslationUnit
            // declarations
            ||  ck == CXCursor_FunctionDecl
            ||  ck == CXCursor_StructDecl
            ||  ck == CXCursor_ClassDecl
    );
}

bool LibClangUtil::isCursorUeContent(CXCursor c){
    CXCursorKind ck = clang_getCursorKind(c);
    return clang_isUnexposed(ck);
}

bool LibClangUtil::isTwoCursorEqual(const CXCursor& c1, const CXCursor& c2){
    return (
            (c1.kind == c2.kind)
        &&  (c1.xdata == c2.xdata)
        &&  (*(c1.data) == *(c2.data))
    );
}

bool LibClangUtil::isCursorDecl(CXCursorKind ck){
    return (    ck == CXCursor_VarDecl
            ||  ck == CXCursor_ParmDecl
            //||  ck == CXCursor_FunctionDecl
    );
}

bool LibClangUtil::isCursorDecl(CXCursor c){
    return isCursorDecl(clang_getCursorKind(c));
}

bool LibClangUtil::isCursorRefExpr(CXCursorKind ck){
    return (ck == CXCursor_DeclRefExpr);
}

bool LibClangUtil::isCursorRefExpr(CXCursor c){
    return isCursorRefExpr(clang_getCursorKind(c));
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

