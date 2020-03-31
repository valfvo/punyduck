#pragma once

#include <utility>
#include <vector>

#include "LQDeclaration.hpp"

class LQEvent {
public:
    virtual ~LQEvent();
};

struct LQModelUpdateEvent {
    const std::vector<std::pair<const char*, LQindex>> infos;
};
