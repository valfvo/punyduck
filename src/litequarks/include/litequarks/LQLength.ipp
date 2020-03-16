#include <algorithm>
#include "LQLength.hpp"
#include <iostream>

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit>::LQLength()
: m_quark(nullptr),
  m_lengthGetter(nullptr), m_lengthSetter(nullptr),
  m_coefficient(1.0f), m_variable(0.0f),
  m_parent(nullptr), m_firstChild(nullptr), m_lastChild(nullptr),
  m_prevSibling(nullptr), m_nextSibling(nullptr)
{
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit>::LQLength(
    TQuark* quark,
    LQGetterPointer<TQuark, TUnit> lengthGetter,
    LQSetterPointer<TQuark, TUnit> lengthSetter,
    TUnit coefficient,
    TUnit variable)
: m_quark(quark),
  m_lengthGetter(lengthGetter), m_lengthSetter(lengthSetter),
  m_coefficient(coefficient), m_variable(variable),
  m_parent(nullptr), m_firstChild(nullptr), m_lastChild(nullptr),
  m_prevSibling(nullptr), m_nextSibling(nullptr)
{
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit>::LQLength(
    TQuark* quark,
    LQGetterPointer<TQuark, TUnit> lengthGetter,
    LQSetterPointer<TQuark, TUnit> lengthSetter,
    TUnit coefficient,
    TUnit variable,
    LQLength<TQuark, TUnit>* parent,
    LQLength<TQuark, TUnit>* firstChild,
    LQLength<TQuark, TUnit>* lastChild,
    LQLength<TQuark, TUnit>* prevSibling,
    LQLength<TQuark, TUnit>* nextSibling)
: m_quark(quark),
  m_lengthGetter(lengthGetter), m_lengthSetter(lengthSetter),
  m_coefficient(coefficient), m_variable(variable),
  m_parent(parent), m_firstChild(firstChild), m_lastChild(lastChild),
  m_prevSibling(prevSibling), m_nextSibling(nextSibling)
{
}

template<class TQuark, class TUnit>
void LQLength<TQuark, TUnit>::linkToQuark(
    TQuark* quark,
    LQGetterPointer<TQuark, TUnit> lengthGetter,
    LQSetterPointer<TQuark, TUnit> lengthSetter
) {
    m_quark = quark;
    m_lengthGetter = lengthGetter;
    m_lengthSetter = lengthSetter;
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> LQLength<TQuark, TUnit>::mock() {
    LQLength<TQuark, TUnit> mockLength(*this);
    mockLength.m_parent = this;
    return mockLength;
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit>& LQLength<TQuark, TUnit>::operator=(TUnit unit) {
    m_parent->detach();
    m_parent->m_coefficient = 1.0f;
    m_parent->m_variable = 0.0f;
    (m_quark->*m_lengthSetter)(unit);
    if (m_parent->m_firstChild) {
        m_parent->m_firstChild->resizeCallback();
    }
    return *this;
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit>& LQLength<TQuark, TUnit>::operator=(LQLength<TQuark, TUnit> length) {
    // detach();
    m_parent->m_coefficient = length.m_coefficient;
    m_parent->m_variable = length.m_variable;
    // std::cout <<"parent:"<<m_parent << std::endl;
    length.m_parent->appendChild(m_parent);
    // std::cout <<"coeff:" <<m_coefficient << std::endl;
    // (m_quark->*m_lengthSetter)(*this);
    // resizeCallback();
    return *this;
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> LQLength<TQuark, TUnit>::operator+(TUnit variable) const {
    return LQLength<TQuark, TUnit>(
        m_quark, m_lengthGetter, m_lengthSetter,
        m_coefficient, m_variable + variable,
        m_parent, m_firstChild, m_lastChild, m_prevSibling, m_nextSibling);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> LQLength<TQuark, TUnit>::operator-(TUnit variable) const {
    return operator+(-variable);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> LQLength<TQuark, TUnit>::operator*(TUnit coefficient) const {
    LQLength<TQuark, TUnit> length(*this);
    length.m_coefficient *= coefficient;
    length.m_variable *= coefficient;
    return length;
    // return LQLength<TQuark, TUnit>(
    //     m_quark, m_lengthGetter, m_lengthSetter,
    //     m_coefficient * coefficient, m_variable * coefficient,
    //     m_parent, m_firstChild, m_lastChild, m_prevSibling, m_nextSibling);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> LQLength<TQuark, TUnit>::operator/(TUnit coefficient) const {
    return operator*(1.0f / coefficient);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit>::operator TUnit() const {
    return (m_quark->*m_lengthGetter)();
}

template<class TQuark, class TUnit>
void LQLength<TQuark, TUnit>::appendChild(LQLength<TQuark, TUnit>* child) {
    child->detach();
    child->m_parent = this;
    if (m_lastChild) {
        m_lastChild->m_nextSibling = child;
        child->m_prevSibling = m_lastChild;
        m_lastChild = child;
    }
    else {
        m_firstChild = child;
        m_lastChild = child;
    }
    child->resizeCallback();
}

template<class TQuark, class TUnit>
void LQLength<TQuark, TUnit>::detach() {
    if (m_parent) {
        if (m_prevSibling) {
            m_prevSibling->m_nextSibling = m_nextSibling;
        }
        else {
            m_parent->m_firstChild = m_nextSibling;
        }

        if (m_nextSibling) {
            m_nextSibling->m_prevSibling = m_prevSibling;
        }
        else {
            m_parent->m_lastChild = m_prevSibling;
        }

        m_parent = nullptr;
        m_prevSibling = nullptr;
        m_nextSibling = nullptr;
    }
}

template<class TQuark, class TUnit>
void LQLength<TQuark, TUnit>::resizeCallback() {
    (m_quark->*m_lengthSetter)(m_coefficient * static_cast<TUnit>(*m_parent) + m_variable);
    for (
        auto child = m_firstChild;
        child != nullptr;
        child = child->m_nextSibling
    ) {
        child->resizeCallback();
    }
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator+(TUnit unit, LQLength<TQuark, TUnit> length) {
    return length.operator+(unit);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator-(TUnit unit, LQLength<TQuark, TUnit> length) {
    return length.operator-(unit);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator*(TUnit unit, LQLength<TQuark, TUnit> length) {
    return length.operator*(unit);
}

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator/(TUnit unit, LQLength<TQuark, TUnit> length) {
    return length.operator/(unit);
}
