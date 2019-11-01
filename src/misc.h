#ifndef CPPSYNTH_MISC_H
#define CPPSYNTH_MISC_H

#include <src/LibClangUtil.h>
#include <src/CurInfo.h>

#include <clang-c/Index.h>

#include <functional>

namespace std {
    template<>
    struct equal_to<CXCursor>
    {
        bool operator()(const CXCursor& c1, const CXCursor& c2) const {
            return (
                        (c1.kind == c2.kind)
                    &&  (c1.xdata == c2.xdata)
                    &&  (*(c1.data) == *(c2.data))
                );
        }
    };
    
    template <>
    struct hash<CXCursor>
    {
        std::size_t operator()(const CXCursor& c) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            unsigned long long cd = reinterpret_cast<unsigned long long>(*(c.data));
            CurInfo ci(c);
            unsigned start = ci.range.begin_int_data;
            unsigned  end = ci.range.end_int_data;
            unsigned col = ci.column;
            unsigned line = ci.line;

            return(
                  (hash<int>()(start) << hash<int>()(end % 3))
                ^ (hash<int>()(end) << hash<int>()(c.kind % 5))
                ^ (hash<unsigned long long>()(cd) << hash<int>()(start % 7))
                ^ (hash<int>()(c.kind) << hash<int>()(line % 8))
                ^ (hash<int>()(line) << hash<int>()(col % 12))
                ^ (hash<int>()(col) << hash<int>()(start % 13))
            );
        }
    };

}

#endif