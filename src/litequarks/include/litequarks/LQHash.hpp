#pragma once

#include <functional>  // std::hash

template<class T>
inline void lqHashCombine(std::size_t &seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

#ifndef LQ_NO_PAIR_HASH
    namespace std {
        template<class T1, class T2>
        struct hash<std::pair<T1, T2>> {
            size_t operator()(const std::pair<T1, T2>& p) const {
                size_t seed = 0;
                lqHashCombine(seed, p.first);
                lqHashCombine(seed, p.second);

                return seed;
            }
        };
    }
#endif
