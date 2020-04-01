#pragma once

#include <cstddef>

using LQindex = std::ptrdiff_t;
using LQsize  = std::ptrdiff_t;
using LQenum  = int;

using LQMetricKind = int;
#define LQMetricKind_Distance 0x1000
#define LQMetricKind_Length   0x1001
#define LQMetricKind_Coords   0x1002

#define LQ_CREATE_MODEL(name)\
    LQAppModel::addModelCreator(#name, [](LQRawData lq_rawData) -> void*

#define LQ_PARSE(type)\
    lq_rawData.parse<type>()

template<class TQuark, class TMember>
using LQGetterPtr = TMember (TQuark::*)();

template<class TQuark, class TMember>
using LQSetterPtr = void (TQuark::*)(TMember);

class LQRawData;
using LQModelCreator = void* (*)(LQRawData&);

class LQViewable;
class LQModelData;
using LQView = LQViewable;
using LQViewCreator = LQView (*)(LQModelData);

#define LQwu 50.0f
#define LQhu 50.0f

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
}
