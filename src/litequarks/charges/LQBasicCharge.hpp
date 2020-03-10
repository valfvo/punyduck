#pragma once

template<class T>
class LQBasicCharge {
public:
    LQBasicCharge(T& quark);
    LQBasicCharge(T& quark, bool state);
    bool state();
    void enable();
    void disable();
    void reverse();

protected:
    bool m_state;
    T& m_quark;
};

#include "LQBasicCharge.ipp"
