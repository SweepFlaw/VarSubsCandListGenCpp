# VarSubsCandListGenCpp
Variable Substitution Candidate List Generator for a Cpp file.

## HOW TO USE
```
#
# <compile>
#
make

#
# <run examples>
#
# ./bin/vscm [input-file] [output-file] [main-function-mode (for now, just 0)] [candidate-select-mode] 

# candidate-select-mode=0 : no-filter
bin/vscm test/tc.cpp test/log8.txt 0 0
# candidate-select-mode=1 : exact-type-filter
bin/vscm test/tc.cpp test/log9.txt 0 1
```

## OUTPUT FILE FORMAT
Written in csv format, it separates 10 datas in one line. Lines are separated with newline `\n` character. The following is a sequence of 10 datas.
1. Target-Ref Line `unsigned`
2. Target-Ref Column `unsigned`
3. Target-Ref Offset `unsigned`
4. Target-Var Length (target variable's length) `size_t`
5. Target Variable `string`
6. Candidate-Def Line `unsigned`
7. Candidate-Def Column `unsigned`
8. Candidate-Def Offset `unsigned`
9. Candidate-Var Length `size_t`
10. Candidate Variable `string`
