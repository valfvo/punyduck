#pragma once

#include <cstddef>

using LQindex = std::ptrdiff_t;
using LQsize  = std::ptrdiff_t;
using LQenum  = int;

template<class TQuark, class TMember>
using LQGetterPointer = TMember (TQuark::*)();

template<class TQuark, class TMember>
using LQSetterPointer = void (TQuark::*)(TMember);
