#include <algorithm>
#include "LQMetric.hpp"
#include <iostream>

template<class TQuark, class TUnit>
LQMetric<TQuark, TUnit>::
LQMetric()
: m_quark(nullptr),
  m_getter(nullptr), m_setter(nullptr),
  m_kind(LQMetricKind_Distance),
  m_expr(nullptr, 0.0f), m_refs() { }

template<class TQuark, class TUnit>
LQMetric<TQuark, TUnit>::
LQMetric(LQMetricKind kind)
: m_quark(nullptr),
  m_getter(nullptr), m_setter(nullptr),
  m_kind(kind),
  m_expr(nullptr, 0.0f), m_refs() { }

template<class TQuark, class TUnit>
LQMetric<TQuark, TUnit>::
LQMetric(TQuark* quark,
    LQGetterPtr<TQuark, TUnit> getter,
    LQSetterPtr<TQuark, TUnit> setter)
: m_quark(quark),
  m_getter(getter), m_setter(setter),
  m_kind(LQMetricKind_Distance),
  m_expr(nullptr, 0.0f), m_refs() { }

template<class TQuark, class TUnit>
void
LQMetric<TQuark, TUnit>::
linkToQuark(TQuark* quark,
    LQGetterPtr<TQuark, TUnit> getter,
    LQSetterPtr<TQuark, TUnit> setter)
{
    m_quark = quark;
    m_getter = getter;
    m_setter = setter;
}

template<class TQuark, class TUnit>
void
LQMetric<TQuark, TUnit>::
recalculate()
{
    (m_quark->*m_setter)(m_expr.evaluate());
    for (auto ref : m_refs) {
        ref->recalculate();
    }
}

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> LQMetric<TQuark, TUnit>::mock() {
//     LQMetric<TQuark, TUnit> mockLength(*this);
//     mockLength.m_parent = this;
//     return mockLength;
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit>& LQMetric<TQuark, TUnit>::operator=(TUnit unit) {
//     m_parent->detach();
//     m_parent->m_coefficient = 1.0f;
//     m_parent->m_variable = 0.0f;
//     (m_quark->*m_setter)(unit);
//     if (m_parent->m_firstChild) {
//         m_parent->m_firstChild->resizeCallback();
//     }
//     return *this;
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit>& LQMetric<TQuark, TUnit>::operator=(LQMetric<TQuark, TUnit> length) {
//     // detach();
//     m_parent->m_coefficient = length.m_coefficient;
//     m_parent->m_variable = length.m_variable;
//     // std::cout <<"parent:"<<m_parent << std::endl;
//     length.m_parent->appendChild(m_parent);
//     // std::cout <<"coeff:" <<m_coefficient << std::endl;
//     // (m_quark->*m_setter)(*this);
//     // resizeCallback();
//     return *this;
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> LQMetric<TQuark, TUnit>::operator+(TUnit variable) const {
//     LQMetric<TQuark, TUnit> length(*this);
//     length.m_variable += variable;
//     return length;
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> LQMetric<TQuark, TUnit>::operator-(TUnit variable) const {
//     return operator+(-variable);
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> LQMetric<TQuark, TUnit>::operator*(TUnit coefficient) const {
//     LQMetric<TQuark, TUnit> length(*this);
//     length.m_coefficient *= coefficient;
//     length.m_variable *= coefficient;
//     return length;
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> LQMetric<TQuark, TUnit>::operator/(TUnit coefficient) const {
//     return operator*(1.0f / coefficient);
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit>::
// operator TUnit() const
// {
//     return (m_quark->*m_getter)();
// }

template<class TQuark, class TUnit>
LQMetric<TQuark, TUnit>::
operator LQMetricExpr<TQuark, TUnit>()
{
    return LQMetricExpr<TQuark, TUnit>(this);
}

// template<class TQuark, class TUnit>
// void LQMetric<TQuark, TUnit>::appendChild(LQMetric<TQuark, TUnit>* child) {
//     child->detach();
//     child->m_parent = this;
//     if (m_lastChild) {
//         m_lastChild->m_nextSibling = child;
//         child->m_prevSibling = m_lastChild;
//         m_lastChild = child;
//     }
//     else {
//         m_firstChild = child;
//         m_lastChild = child;
//     }
//     child->resizeCallback();
// }

// template<class TQuark, class TUnit>
// void LQMetric<TQuark, TUnit>::detach() {
//     if (m_parent) {
//         if (m_prevSibling) {
//             m_prevSibling->m_nextSibling = m_nextSibling;
//         }
//         else {
//             m_parent->m_firstChild = m_nextSibling;
//         }

//         if (m_nextSibling) {
//             m_nextSibling->m_prevSibling = m_prevSibling;
//         }
//         else {
//             m_parent->m_lastChild = m_prevSibling;
//         }

//         m_parent = nullptr;
//         m_prevSibling = nullptr;
//         m_nextSibling = nullptr;
//     }
// }

// template<class TQuark, class TUnit>
// void LQMetric<TQuark, TUnit>::resizeCallback() {
//     (m_quark->*m_setter)(m_coefficient * static_cast<TUnit>(*m_parent) + m_variable);
//     for (
//         auto child = m_firstChild;
//         child != nullptr;
//         child = child->m_nextSibling
//     ) {
//         child->resizeCallback();
//     }
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator+(TUnit unit, LQMetric<TQuark, TUnit> length) {
//     return length.operator+(unit);
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator-(TUnit unit, LQMetric<TQuark, TUnit> length) {
//     return length.operator-(unit);
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator*(TUnit unit, LQMetric<TQuark, TUnit> length) {
//     return length.operator*(unit);
// }

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator/(TUnit unit, LQMetric<TQuark, TUnit> length) {
//     return length.operator/(unit);
// }
