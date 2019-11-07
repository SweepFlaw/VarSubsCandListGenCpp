#include <src/CppTree.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// global variables

int makeVarCandidate(string ifn, string ofn, int csm=0, string separator=",");

int main(int argc, char** argv){
    // <Command Line Argument Scheme>
    //      0. [program-name]
    //      1. [target-source-filename (type:string. detail:include-path)]
    //      2. [output-file-name (type:string. detail:include-path)]
    //      3. [main-function-mode (type:int.)]
    //      4. [candidate-select-mode (type:int. detail:optional)]

    if(argc < 4){
        //print usage and exit
        cerr << "ERROR: at least 3 arguments needed." << endl;
        cerr << "Usage: [program-name] [target-source-filename] [main-function-mode] [output-filename] [candidate-select-mode(optional)]" << endl;
        exit(-1);
    }


    int mainFuncMode;
    int candSelMode=0;
    string targetFilename;
    string outputFilename;


    targetFilename = static_cast<string>(argv[1]);
    outputFilename = static_cast<string>(argv[2]);
    mainFuncMode = std::stoi(argv[3]);
    if(argc > 4){
        candSelMode = std::stoi(argv[4]);
    }

    switch(mainFuncMode){
        // get
        case 0:
            makeVarCandidate(targetFilename, outputFilename, candSelMode);
            break;

        default:
            cerr << "Mode number not matched. Write Appropriate mode number." << endl;
            exit(-1);
    }

    return 0;
}

// output Format : 10 columns, separated by separator and newline('\n') characters.
//      1. Target-Ref Line (unsigned)
//      2. Target-Ref Column (unsigned)
//      3. Target-Ref Offset (unsigned)
//      4. Target-Var Length (target variable's length. size_t)
//      5. Target Variable (string)
//      6. Candidate-Def Line (unsigned)
//      7. Candidate-Def Column (unsigned)
//      8. Candidate-Def Offset (unsigned)
//      9. Candidate-Var Length (size_t)
//      10. Candidate Variable (string)
int makeVarCandidate(string ifn, string ofn, int csm, string separator){
    ofstream ofs(ofn, std::ofstream::out);
    const string lineSeparator = "\n";

    CppTree ct(ifn.c_str());
    CppTree::varSubsCandMap vscm = ct.getVarSubsCandMap(csm);
    for(auto& p : vscm){
        CurInfo cip(p.first);
        for(auto& q: p.second){
            CurInfo ciq(q);
            ofs <<  cip.line    <<  separator
                <<  cip.column  <<  separator
                <<  cip.offset  <<  separator
                <<  cip.curStr.length() <<  separator
                <<  cip.curStr  <<  separator
                <<  ciq.line    <<  separator
                <<  ciq.column  <<  separator
                <<  ciq.offset  <<  separator
                <<  ciq.curStr.length() <<  separator
                <<  ciq.curStr
                <<  lineSeparator
            ;
        }
    }
    ofs.close();

    return 0;
}