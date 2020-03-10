#include "LQBasicCharge.hpp"

template<class T>
LQBasicCharge<T>::LQBasicCharge(T& quark)
: m_quark(quark), m_state(true)
{
}

template<class T>
LQBasicCharge<T>::LQBasicCharge(T& quark, bool state)
: m_quark(quark), m_state(state)
{
}

template<class T>
bool LQBasicCharge<T>::state() {
    return m_state;
}

template<class T>
void LQBasicCharge<T>::enable() {
    m_state = true;
}

template<class T>
void LQBasicCharge<T>::disable() {
    m_state = false;
}

template<class T>
void LQBasicCharge<T>::reverse() {
    m_state = !m_state;
}
