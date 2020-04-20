#include <algorithm>
#include "LQNumber.hpp"
#include <iostream>

// template<class TQuark, class TUnit>
// LQNumber::
// LQNumber()
// : m_quark(nullptr),
// //   m_getter(nullptr), m_setter(nullptr),
//   m_kind(LQMetricKind_Distance),
//   m_expr(nullptr, 0.0f), m_refs() { }

// template<class TQuark, class TUnit>
// LQNumber<TQuark, TUnit>::
// LQNumber(LQMetricKind kind)
// : m_quark(nullptr),
//   m_getter(nullptr), m_setter(nullptr),
//   m_kind(kind),
//   m_expr(nullptr, 0.0f), m_refs() { }

// template<class TQuark, class TUnit>
// LQNumber<TQuark, TUnit>::
// LQNumber(TQuark* quark,
//     LQGetterPtr<TQuark, TUnit> getter,
//     LQSetterPtr<TQuark, TUnit> setter)
// : m_quark(quark),
//   m_getter(getter), m_setter(setter),
//   m_kind(LQMetricKind_Distance),
//   m_expr(nullptr, 0.0f), m_refs() { }

// template<class TQuark, class TUnit>
// void
// LQNumber<TQuark, TUnit>::
// linkToQuark(TQuark* quark,
//     LQGetterPtr<TQuark, TUnit> getter,
//     LQSetterPtr<TQuark, TUnit> setter)
// {
//     m_quark = quark;
//     m_getter = getter;
//     m_setter = setter;
// }

// void LQNumber::recalculate()
// {
//     (m_quark->*m_setter)(m_expr.evaluate());
//     for (auto ref : m_refs) {
//         ref->recalculate();
//     }
// }

// template<class TQuark, class TUnit>
// LQNumber<TQuark, TUnit>::
// operator TUnit() const
// {
//     return (m_quark->*m_getter)();
// }

// LQNumber::operator LQMathExpr()
// {
//     return LQMathExpr(this);
// }

// template<class TQuark, class TUnit>
// void LQNumber<TQuark, TUnit>::appendChild(LQNumber<TQuark, TUnit>* child) {
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
// void LQNumber<TQuark, TUnit>::detach() {
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
// void LQNumber<TQuark, TUnit>::resizeCallback() {
//     (m_quark->*m_setter)(m_coefficient * static_cast<TUnit>(*m_parent) + m_variable);
//     for (
//         auto child = m_firstChild;
//         child != nullptr;
//         child = child->m_nextSibling
//     ) {
//         child->resizeCallback();
//     }
// }
