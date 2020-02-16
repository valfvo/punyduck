#pragma once

#include "LQDeclaration.hpp"
#include "LQEventLinker.hpp"
#include "LQEventCharge.hpp"

class LQuark {
public:
    LQuark(uint64_t code);
    LQuark(uint64_t code, LQEventCallback& callbacks);
    virtual ~LQuark();

    LQEventCharge event;
};
