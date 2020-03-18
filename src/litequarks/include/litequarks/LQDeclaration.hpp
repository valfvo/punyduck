#pragma once

#include <cstddef>

using LQindex = std::ptrdiff_t;
using LQsize  = std::ptrdiff_t;
using LQenum  = int;

using LQMetricKind = int;
#define LQMetricKind_Distance 0x1000
#define LQMetricKind_Length   0x1001
#define LQMetricKind_Coords   0x1002

template<class TQuark, class TMember>
using LQGetterPtr = TMember (TQuark::*)();

template<class TQuark, class TMember>
using LQSetterPtr = void (TQuark::*)(TMember);
