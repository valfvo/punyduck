#pragma once

#include <cstddef>

using LQindex = std::ptrdiff_t;
using LQsize  = std::ptrdiff_t;
using LQenum  = int;
using LQitem  = void;

using LQMetricKind = int;
#define LQMetricKind_Distance 0x1000
#define LQMetricKind_Length   0x1001
#define LQMetricKind_Coords   0x1002

template<class TQuark, class TMember>
using LQGetterPtr = TMember (TQuark::*)();

template<class TQuark, class TMember>
using LQSetterPtr = void (TQuark::*)(TMember);

class LQRawData;
using LQItemCreator = void* (*)(LQRawData&, const bool*);

struct LQImageData {
    int width;
    int height;
    int format;
    unsigned char* pixels;
};

class LQViewable;
class LQModelData;
using LQView = LQViewable;
using LQViewCreator = LQView (*)(LQModelData);

#define LQwu 50.0f
#define LQhu 50.0f
#define LQpx  1.0f
#define LQem (16.0f * LQpx)
#define LQpt (16.0f / 12.0f * LQpx)

namespace LQUnit {
    constexpr float operator "" _wu(long double unit) {
        return unit * LQwu;
    }

    constexpr float operator "" _wu(unsigned long long unit) {
        return unit * LQwu;
    }

    constexpr float operator "" _hu(long double unit) {
        return unit * LQhu;
    }

    constexpr float operator "" _hu(unsigned long long unit) {
        return unit * LQhu;
    }

    constexpr float operator "" _px(long double unit) {
        return unit * LQpx;
    }

    constexpr float operator "" _px(unsigned long long unit) {
        return unit * LQpx;
    }

    constexpr float operator "" _em(long double unit) {
        return unit * LQem;
    }

    constexpr float operator "" _em(unsigned long long unit) {
        return unit * LQem;
    }

    constexpr float operator "" _pt(long double unit) {
        return unit * LQpt;
    }

    constexpr float operator "" _pt(unsigned long long unit) {
        return unit * LQpt;
    }
}
