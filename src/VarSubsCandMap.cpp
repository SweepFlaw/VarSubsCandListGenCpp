#include <src/VarSubsCandMap.h>

using namespace std;
using vSubsCandMap = std::unordered_map<CXCursor, std::vector<CXCursor>>;
using curTree = std::unordered_map<CXCursor, std::vector<CXCursor>>;

static curTree cursorTree; 
static std::vector<std::pair<CXCursor, std::vector<CXCursor>>> scstack;
static vSubsCandMap vscm;

static CXChildVisitResult vscm_visitFunc(CXCursor c, CXCursor parent, CXClientData client_data){

    //TODO

    // DUMMY CODE
    using cvcp = pair<CXCursor, vector<CXCursor>>;
    scstack.push_back(cvcp(c, vector<CXCursor>{c}));
    return CXChildVisit_Recurse;
}

vSubsCandMap makeVarSubsCandMap(CXCursor c){

    //TODO

    // DUMMY CODE
    clang_visitChildren(c, vscm_visitFunc, nullptr);
    return vscm;
}
