#include <src/VarInfo.h>

#include <src/LibClangUtil.h>

#include <stdexcept>

using namespace std;

VarInfo::VarInfo(CXCursor c, bool throwException)
    : cursor{c}
    , ckind{clang_getCursorKind(c)}
    , location{clang_getCursorLocation(c)}
    , ctype{clang_getCursorType(c)}
    , varStr{LibClangUtil::getCurStr(c)}
    , initFail{true}
{
    if(ckind == CXCursor_VarDecl || ckind == CXCursor_DeclRefExpr){
        clang_getSpellingLocation(location, &cfile, &line, &column, &offset);
        initFail = false;
    }
    else if (throwException){
        throw invalid_argument("VarInfo::VarInfo - ckind check failed.");
    }
    else{
        line = 0;
        column = 0;
        offset = 0;
    }
}

void VarInfo::printReadableStr(ostream& os){
    os  << "Var name:\t"    << varStr << endl
        << "\tcursor kind:\t"   << LibClangUtil::getCurKindStr(ckind)   << endl
        << "\tcursor type:\t"   << LibClangUtil::getTypeStr(ctype)      << endl
        << "\tLoc Line   :\t"   << line                                 << endl
        << "\tLoc Column :\t"   << column                               << endl
        << "\tLoc Offset :\t"   << offset                               << endl
    ;
}