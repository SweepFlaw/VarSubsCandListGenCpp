#include <src/CppTree.h>
#include <src/CurInfo.h>
#include <src/LibClangUtil.h>
#include <src/misc.h>

#include <iostream>
#include <string>

using namespace std;

int main()
{
    const char *fnc = "test/tc.cpp";
    string fn = static_cast<string>(fnc);
    CppTree ct(const_cast<char*>(fn.c_str()));

    CppTree::curTree ctree1 = ct.getCursorTree();
    CppTree::varScopeMap vsm1 = ct.getVarScopeMap(ctree1);
    CppTree::varSubsCandMap vscm1 = ct.getVarSubsCandMap(ctree1, vsm1);

    // this struct is used for clang_visitChildren function.
    struct ClData {
        string filename;
        CppTree::curTree ctree;
        CppTree::varScopeMap vsm;
    };
    ClData cldata{
        fn,
        ctree1,
        vsm1
    };


    // TEST1 'CppTree::getVarSubsCandMap'
    for(auto& refe : vscm1){
        cout << "================DELIMETER===============" << endl << endl;
        CurInfo(refe.first).printReadableStr(cout);
        cout << "Available variables" << endl;
        for(auto& defc : refe.second){
            CurInfo ci(defc);
            cout << '\t' << ci.curStr << '\t' << "\tLine: " << ci.line << "\tColumn: " << ci.column << "\tType: " << LibClangUtil::getTypeStr(ci.ctype) << endl;
        }
    }

    
    // TEST2 'CppTree::getVarScopeMap'
    //cout << "=====================VARDECL===================" << endl;
    //cout << "=====================VARDECL===================" << endl;
    //cout << "=====================VARDECL===================" << endl;
    //cout << "=====================VARDECL===================" << endl;
    //cout << "=====================VARDECL===================" << endl;
    //for(auto& k : vsm1.varDeclSM){
    //    cout << "================VARDECL_DELIMETER===============" << endl << endl;
    //    CurInfo(k.first).printReadableStr(cout);
    //    CurInfo(k.second).printReadableStr(cout);
    //}
    //cout << "=====================DECLREF===================" << endl;
    //cout << "=====================DECLREF===================" << endl;
    //cout << "=====================DECLREF===================" << endl;
    //cout << "=====================DECLREF===================" << endl;
    //cout << "=====================DECLREF===================" << endl;
    //for(auto& k : vsm1.varDRefSM){
    //    cout << "==========DECLREF_DELIMETER========" << endl << endl;
    //    CurInfo(k.first).printReadableStr(cout);
    //    CurInfo(k.second).printReadableStr(cout);
    //}


    //TEST3 'CppTree::curTree'
    //clang_visitChildren(
    //    ct.cursor,
    //    [](CXCursor c, CXCursor parent, CXClientData client_data)
    //    {
    //        if(LibClangUtil::isCursorOnFile(c, "test/tc.cpp")){
    //            CurInfo ci (c);
    //            if(!LibClangUtil::isCursorUeContent(c)){
    //                cout << "================DELIMETER===============" << endl << endl;
    //                CurInfo p (c);
    //                int counter = 0;
    //                auto& parentInfo = ((ClData*)(client_data))->ctree.parent;
    //                while( true){
    //                    counter++;
    //                    p.printReadableStr(cout);
    //                    auto findSuccess = parentInfo.find(p.cursor);
    //                    if(findSuccess == parentInfo.end()){
    //                        break;
    //                    }
    //                    else{
    //                        p = CurInfo(findSuccess->second);
    //                    }
    //                }
    //            }
    //        }
    //    
    //        return CXChildVisit_Recurse;
    //    },
    //    &cldata
    //);


    return 0;
}
