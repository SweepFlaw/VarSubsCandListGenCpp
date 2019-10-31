#ifndef CPPSYNTH_VARSUBSCANDMAP_H
#define CPPSYNTH_VARSUBSCANDMAP_H

#include <clang-c/Index.h>

#include <unordered_map>
#include <utility>
#include <vector>

namespace std {
    // insert hash function for std::unordered_map<CXCursor, _>
    template <>
    struct hash<CXCursor>
    {
        std::size_t operator()(const CXCursor& c) const
        {
        using std::size_t;
        using std::hash;
        using std::string;

        return(  (hash<int>()(c.xdata)
               ^ (hash<int*>()((int*)(c.data)) << 1)) >> 1);
        }
    };
}

std::unordered_map<CXCursor, std::vector<CXCursor>> makeVarSubsCandMap(CXCursor c);



#endif