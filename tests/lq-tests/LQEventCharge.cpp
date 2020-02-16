#include "LQEventCharge.hpp"

LQEventCharge::LQEventCharge(LQuark& quark, uint64_t code, LQEventCallback& callbacks)
: m_quark(quark), eventCodeListener(code),
  propagateEvent(true), propagateCaughtEvent(true),
  eventCallbacks(&callbacks)
{
}

void LQEventCharge::listener(LQEvent& event) {
    if (this->eventCodeListener & event.code) {
        eventCallback(event);
        if (this->propagateCaughtEvent) {
            // propagate below
        }
    }
    if (this->propagateEvent) {
        // propagate below
    }
}

LQEventCallback LQEventCharge::getEventCallback(unsigned int code) const {
    return eventCallbacks[code];
}

LQEventCallback LQEventCharge::eventCallback(LQEvent& event) {
    auto callback = getEventCallback(event.code);
    if (callback) {
        (m_quark.*callback)(event);
    }
    return callback;
}
