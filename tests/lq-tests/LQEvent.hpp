#pragma once

#include "LQDeclaration.hpp"

class LQEvent {
public:
    LQEvent(uint64_t code);

    uint64_t const code;
};
