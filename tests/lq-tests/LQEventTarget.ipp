#include "LQEventTarget.hpp"

template<typename T>
LQEventTarget::LQEventTarget(T& target)
: LQEventTarget(target, 0b11)
{
}

template<typename T>
LQEventTarget::LQEventTarget(T& target, uint64_t code)
: LQuark(code, LQEventLinker<T>::data())
{
    if (!LQEventLinker<T>::linked()) {
        linkEventCallbacks<T>();
    }
}

LQEventTarget::~LQEventTarget() {
}

void LQEventTarget::nullEventCallback(LQEvent& event) {
}

void LQEventTarget::mouseMoveCallback(LQEvent& event) {
}

void LQEventTarget::mouseClickCallback(LQEvent& event) {
}

template<typename T>
int LQEventTarget::linkEventCallbacks() const {
    LQEventLinker<T>::link(0, static_cast<LQEventCallback>(nullEventCallback));
    LQEventLinker<T>::link(1, static_cast<LQEventCallback>(mouseMoveCallback));
    LQEventLinker<T>::link(2, static_cast<LQEventCallback>(mouseClickCallback));
    return 0;
}
