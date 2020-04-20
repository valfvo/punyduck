#include <iostream>
#include "LQMathVar.hpp"

// template<class TQuark, class float>
LQMathVar::LQMathVar(
    LQNumber* metric,
    float coeff,
    LQMathVar* nextVar)
: m_metric(metric), m_coeff(coeff), m_nextVar(nextVar)
{
}

// template<class TQuark, class float>
void LQMathVar::setNextVar(LQMathVar* nextVar) {
    m_nextVar = nextVar;
}

// template<class TQuark, class float>
float
LQMathVar::
evaluate() const
{
    // return m_coeff * (m_metric->m_quark->*(m_metric->m_getter))();
}

// template<class TQuark, class float>
bool
LQMathVar::
validWith(const LQNumber& metric) const
{
    return m_coeff != 0.0f &&
           (m_metric->m_kind != LQMetricKind_Coords ||
           metric.m_quark->parent() != m_metric->m_quark);
}
