#pragma once

#include "LQDeclaration.hpp"

template<typename T>
class LQEventLinker {
public:
    static int link(unsigned int code, LQEventCallback callback);
    static LQEventCallback& data();
    static bool linked();

protected:
    static LQEventCallback s_eventCallbacks[64];
};

#include "LQEventLinker.ipp"
