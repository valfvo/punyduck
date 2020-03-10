#pragma once

#include "LQDeclaration.hpp"
#include "LQuark.hpp"
#include "LQBasicCharge.hpp"

class LQRedCharge : public LQBasicCharge<LQuark> {
public:
    LQuark& itself();
    LQuark& firstChild();
    LQuark& lastChild();
    LQuark& child(LQuark& child);
    LQuark& child(LQIndex index);
    LQuark& allChilds();
};
