#include "LQEventLinker.hpp"

template<typename T>
int LQEventLinker<T>::link(unsigned int code, LQEventCallback callback) {
        if (code < 64) {
            s_eventCallbacks[code] = callback;
            return 0;
        }
        else {
            return 1;
        }
    }

template<typename T>
LQEventCallback& LQEventLinker<T>::data() {
    return *s_eventCallbacks;
}

template<typename T>
bool LQEventLinker<T>::linked() {
    return s_eventCallbacks[0] != nullptr;
}

template<typename T>
LQEventCallback LQEventLinker<T>::s_eventCallbacks[64] = {};
