#include <src/CurInfo.h>

#include <src/LibClangUtil.h>

#include <stdexcept>

using namespace std;

CurInfo::CurInfo(CXCursor c)
    : cursor{c}
    , ckind{clang_getCursorKind(c)}
    , location{clang_getCursorLocation(c)}
    , range{clang_getCursorExtent(c)}
    , ctype{clang_getCursorType(c)}
    , curStr{LibClangUtil::getCurStr(c)}
{
    isVariable = (ckind == CXCursor_VarDecl || ckind == CXCursor_DeclRefExpr);
    isInvalid = (ckind == CXCursor_InvalidFile);
    clang_getSpellingLocation(location, &cfile, &line, &column, &offset);
}

void CurInfo::printReadableStr(ostream& os){
    os  << "Cur String:\t"    << curStr << endl
        << "\tcursor kind:\t"   << LibClangUtil::getCurKindStr(ckind)   << endl
        << "\tcursor type:\t"   << LibClangUtil::getTypeStr(ctype)      << endl
        << "\tLoc Line   :\t"   << line                                 << endl
        << "\tLoc Column :\t"   << column                               << endl
        << "\tLoc Offset :\t"   << offset                               << endl
        << "\tRange Begin:\t"   << range.begin_int_data                 << endl
        << "\tRange End  :\t"   << range.end_int_data                   << endl
        << "\tIsVariable :\t"   << isVariable                           << endl
    ;
}