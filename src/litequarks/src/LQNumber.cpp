#include <algorithm>

#include <litequarks/LQNumber.hpp>

float LQNumber::old = 0.0f;

LQNumber::LQNumber()
: m_quark(nullptr), m_invoke(nullptr), m_value(0.0f),
  m_kind(LQNumber::Kind::value), m_expr(), m_refs() { }

LQNumber::LQNumber(float value)
: m_quark(nullptr), m_invoke(nullptr), m_value(value),
  m_kind(LQNumber::Kind::value), m_expr(), m_refs() { }

LQNumber::LQNumber(LQMathExpr&& expr)
: m_quark(nullptr), m_invoke(nullptr), m_value(expr.eval()),
  m_kind(LQNumber::Kind::value), m_expr(std::move(expr)), m_refs() { }

LQNumber::LQNumber(LQNumber&& other)
: m_quark(other.m_quark), m_invoke(other.m_invoke), m_value(other.m_value),
  m_kind(other.m_kind), m_expr(std::move(other.m_expr)),
  m_refs(std::move(other.m_refs)) { }

void LQNumber::recalc() {
    old = m_value;
    m_value = m_expr.eval();
    if (m_invoke) {
        m_invoke(m_quark);
    }
    for (auto* p_ref : m_refs) {
        p_ref->recalc();
    }
}

float LQNumber::f() const {
    return m_value;
}

LQNumber::operator float() const {
    return m_value;
}
