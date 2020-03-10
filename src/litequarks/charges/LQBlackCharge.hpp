#pragma once

#include "LQuark.hpp"
#include "LQBasicCharge.hpp"

class LQBlackCharge : public LQBasicCharge<LQuark> {
public:
    /*
        T* parent() const {
            return LQuark.all[m_quark_id];
        }
    */
    // surface.swap.nextSibling();
    LQuark& swap(LQuark const& quark);
    LQuark& swapNextSibling();
    LQuark& swapPrevSibling();
    LQuark& swapFirstChild();
    LQuark& swapLastChild();
};
