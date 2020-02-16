#pragma once

#include "LQDeclaration.hpp"
#include "LQuark.hpp"

class LQEventTarget : public LQuark {
public:
    template<typename T>
    LQEventTarget(T& target);

    template<typename T>
    LQEventTarget(T& target, uint64_t code);

    virtual ~LQEventTarget();

protected:
    virtual void nullEventCallback(LQEvent& event);
    virtual void mouseMoveCallback(LQEvent& event);
    virtual void mouseClickCallback(LQEvent& event);

    template<typename T>
    int linkEventCallbacks() const;
};

#include "LQEventTarget.ipp"
