#include <src/VarSubsCandMap.h>

using namespace std;

static std::vector<std::pair<CXCursor, std::vector<CXCursor>>> scstack;

static CXChildVisitResult vscm_visitFunc(CXCursor c, CXCursor parent, CXClientData client_data){

    //TODO

    // DUMMY CODE
    using cvcp = pair<CXCursor, vector<CXCursor>>;
    scstack.push_back(cvcp(c, vector<CXCursor>{c}));
    return CXChildVisit_Recurse;
}

VarSubsCandMap::VarSubsCandMap(CXCursor c) 
    : vscm{}
{
    //TODO


    // DUMMY CODES
    clang_visitChildren(c, vscm_visitFunc, nullptr);
}
