#pragma once

#include "LQDeclaration.hpp"
#include "LQEvent.hpp"

class LQEventCharge {
public:
    LQEventCharge(LQuark& quark, uint64_t code, LQEventCallback& callbacks);

    void listener(LQEvent& event);

protected:
    LQEventCallback getEventCallback(unsigned int code) const;
    LQEventCallback eventCallback(LQEvent& event);
    // virtual void linkEventCallback(int code, LQEventCallback callback) = 0;

    LQuark& m_quark;
    uint64_t eventCodeListener;
    bool propagateEvent;
    bool propagateCaughtEvent;
    LQEventCallback* eventCallbacks;
};
