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
