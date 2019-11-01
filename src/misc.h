#ifndef CPPSYNTH_MISC_H
#define CPPSYNTH_MISC_H

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
                    &&  (c1.data == c2.data)
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

            return(  (hash<int>()(c.xdata)
                ^ (hash<int*>()((int*)(c.data)) << 1)) >> 1);
        }
    };

}

#endif