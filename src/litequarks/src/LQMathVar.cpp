#include <litequarks/LQMathVar.hpp>
#include <litequarks/LQNumber.hpp>

LQMathVar::LQMathVar(LQNumber& number, float coeff)
: m_number(&number), m_coeff(coeff), m_next(nullptr) { }

void LQMathVar::setNext(LQMathVar* next) {
    m_next = next;
}

float LQMathVar::eval() const {
    return m_coeff * m_number->m_value;
}

bool LQMathVar::parentCoords(const LQNumber& number) const {
    return m_number->m_kind == LQNumber::Kind::coords &&
           m_number->m_quark == number.m_quark->parent();
}

bool LQMathVar::compatible(const LQNumber& number) const {
    return !parentCoords(number) && m_coeff != 0.0f &&
           m_number->m_quark != number.m_quark;
}
