#ifndef CPPSYNTH_VARSUBSCANDMAP_H
#define CPPSYNTH_VARSUBSCANDMAP_H

#include <src/misc.h>

#include <clang-c/Index.h>

#include <unordered_map>
#include <utility>
#include <vector>

std::unordered_map<CXCursor, std::vector<CXCursor>> makeVarSubsCandMap(CXCursor c);

#endif