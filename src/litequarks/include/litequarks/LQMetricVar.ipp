#include <iostream>
#include "LQMetricVar.hpp"

template<class TQuark, class TUnit>
LQMetricVar<TQuark, TUnit>::LQMetricVar(
    LQMetric<TQuark, TUnit>* metric,
    TUnit coeff,
    LQMetricVar<TQuark, TUnit>* nextVar)
: m_metric(metric), m_coeff(coeff), m_nextVar(nextVar)
{
}

template<class TQuark, class TUnit>
void LQMetricVar<TQuark, TUnit>::setNextVar(LQMetricVar<TQuark, TUnit>* nextVar) {
    m_nextVar = nextVar;
}

template<class TQuark, class TUnit>
TUnit
LQMetricVar<TQuark, TUnit>::
evaluate() const
{
    return m_coeff * (m_metric->m_quark->*(m_metric->m_getter))();
}

template<class TQuark, class TUnit>
bool
LQMetricVar<TQuark, TUnit>::
validWith(const LQMetric<TQuark, TUnit>& metric) const
{
    return m_coeff != 0.0f &&
           (m_metric->m_kind != LQMetricKind_Coords ||
           metric.m_quark->parent() != m_metric->m_quark);
}

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator+(TUnit coeff) const {

// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator+(LQMetric<TQuark, TUnit> metric) const {

// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator+(LQMetricVar<TQuark, TUnit> var) const {

// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator+(LQMetricExpr<TQuark, TUnit> expr) const {

// }




// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator-(TUnit coeff) const {

// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator-(LQMetric<TQuark, TUnit> metric) const {

// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator-(LQMetricVar<TQuark, TUnit> var) const {

// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator-(LQMetricExpr<TQuark, TUnit> expr) const {

// }



// template<class TQuark, class TUnit>
// LQMetricVar<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator*(TUnit coeff) const {
//     return LQMetricVar<TQuark, TUnit>(m_coeff*coeff, m_metric, m_nextVar);
// }

// template<class TQuark, class TUnit>
// LQMetricVar<TQuark, TUnit>
// LQMetricVar<TQuark, TUnit>::operator/(TUnit coeff) const {
//     return LQMetricVar<TQuark, TUnit>(m_coeff/coeff, m_metric, m_nextVar);
// }



// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// operator+(TUnit coeff, const LQMetricVar<TQuark, TUnit>& var) {
//     return var.operator+(coeff);
// }

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// operator-(TUnit coeff, const LQMetricVar<TQuark, TUnit>& var) {
//     return var.operator-(coeff);
// }

// template<class TQuark, class TUnit>
// LQMetricVar<TQuark, TUnit>
// operator*(TUnit coeff, const LQMetricVar<TQuark, TUnit>& var) {
//     return var.operator*(coeff);
// }
