#pragma once

#include <vector>

using LQindex = std::ptrdiff_t;
using LQsize  = std::ptrdiff_t;
using LQenum  = int;

#define LQ_RETURN_THIS   0x100
#define LQ_RETURN_PARENT 0x101
#define LQ_RETURN_CHILD  0x102

class LQuark;
class LQEventTarget;
class LQSurface;

class LQWindow;
class LQButton;

class LQEventLinker;
class LQTexture;

class LQEventCharge;
class LQTransformCharge;
class LQDrawCharge;
class LQEffectCharge;

