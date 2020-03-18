#include <iostream>
#include "LQMetricExpr.hpp"

template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>::
LQMetricExpr(LQMetric<TQuark, TUnit>* metric)
: m_firstVar(new LQMetricVar<TQuark, TUnit>(metric))
{ }


template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>::
LQMetricExpr(LQMetricVar<TQuark, TUnit>* firstVar, TUnit constant)
: m_firstVar(firstVar), m_constant(constant)
{ }

template<class TQuark, class TUnit>
TUnit
LQMetricExpr<TQuark, TUnit>::
evaluate() const
{
    TUnit value = m_constant;
    for (auto var = m_firstVar; var; var = var->m_nextVar) {
        value += var->evaluate();
    }
    return value;
}

template<class TQuark, class TUnit>
void
LQMetricExpr<TQuark, TUnit>::
clear()
{
    m_constant = 0.0f;
    auto prevVar = m_firstVar;
    if (prevVar) {
        auto var = prevVar->m_nextVar;
        for (; var; var = var->m_nextVar) {
            delete prevVar;
        }
        delete var;
        m_firstVar = nullptr;
    }
}


template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>&
LQMetricExpr<TQuark, TUnit>::
operator=(TUnit constant)
{
    // TODO: clear refs links
    auto metric = m_firstVar->m_metric;
    metric->m_expr.m_constant = constant;
    metric->recalculate();
    return *this;
}

template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>&
LQMetricExpr<TQuark, TUnit>::
operator=(const LQMetricExpr<TQuark, TUnit>& other)
{
    if (m_firstVar) {
        auto metric = m_firstVar->m_metric;
        metric->m_expr.m_constant = other.m_constant - m_constant;
        auto* p_var = &metric->m_expr.m_firstVar;
        // add validated other.variables to metric & refs links
        auto var = other.m_firstVar;
        for (; var; var = var->m_nextVar) {
            if (var->validWith(*metric)) {
                *p_var = new LQMetricVar<TQuark, TUnit>(*var);
                p_var = &(*p_var)->m_nextVar;
                var->m_metric->m_refs.push_front(metric);
            }
        }
        // add validated this.variables to metric & refs links
        for (var = m_firstVar->m_nextVar; var; var = var->m_nextVar) {
            if (var->validWith(*metric)) {
                *p_var = new LQMetricVar<TQuark, TUnit>(*var);
                (*p_var)->m_coeff *= -1.0f;  // x + y = 0 <=> x = -y
                p_var = &(*p_var)->m_nextVar;
                var->m_metric->m_refs.push_front(metric);
            }
        }
        // recalculation callback
        metric->recalculate();
    }
    return *this;
}

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>&
// LQMetricExpr<TQuark, TUnit>::
// operator-=(const LQMetricExpr<TQuark, TUnit>& other)
// {
//     m_constant -= other.m_constant;
//     if (other.m_firstVar) {
//         if (m_firstVar == nullptr) {
//             m_firstVar = other.m_firstVar;
//         }
//         else if (other.m_firstVar->m_nextVar == nullptr) {
//             other.m_firstVar->m_nextVar = m_firstVar;
//             m_firstVar = other.m_firstVar;
//         }
//         else {  // other has at least 2 variables & this has at least 1 variable
//             auto& last = m_firstVar;
//             while (last->m_nextVar != nullptr) {
//                 last = last->m_nextVar;
//             }
//             last->m_nextVar = other.m_firstVar;
//         }
//         for (auto var = other.m_firstVar; var; var = var->m_nextVar) {
//             var->m_coeff *= -1.0f;
//         }
//     }
//     return *this;
// }

template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>
LQMetricExpr<TQuark, TUnit>::
operator+(TUnit constant) const
{
    return LQMetricExpr<TQuark, TUnit>(m_firstVar, m_constant+constant);
}

template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>
LQMetricExpr<TQuark, TUnit>::
operator+(const LQMetricExpr<TQuark, TUnit>& other) const
{
    LQMetricExpr<TQuark, TUnit> expr(m_firstVar, m_constant+other.m_constant);
    if (other.m_firstVar) {
        if (expr.m_firstVar == nullptr) {
            expr.m_firstVar = other.m_firstVar;
        }
        else if (other.m_firstVar->m_nextVar == nullptr) {
            other.m_firstVar->m_nextVar = expr.m_firstVar;
            expr.m_firstVar = other.m_firstVar;
        }
        else {  // other has at least 2 variables & this has at least 1 variable
            auto& last = expr.m_firstVar;
            while (last->m_nextVar != nullptr) {
                last = last->m_nextVar;
            }
            last->m_nextVar = other.m_firstVar;
        }
    }
    return expr;
}



template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>
LQMetricExpr<TQuark, TUnit>::
operator-(TUnit constant) const
{
    return LQMetricExpr<TQuark, TUnit>(m_firstVar, m_constant-constant);
}

template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>
LQMetricExpr<TQuark, TUnit>::
operator-(const LQMetricExpr<TQuark, TUnit>& other) const
{
    LQMetricExpr<TQuark, TUnit> expr(m_firstVar, m_constant-other.m_constant);
    if (other.m_firstVar) {
        if (expr.m_firstVar == nullptr) {
            expr.m_firstVar = other.m_firstVar;
        }
        else if (other.m_firstVar->m_nextVar == nullptr) {
            other.m_firstVar->m_nextVar = expr.m_firstVar;
            expr.m_firstVar = other.m_firstVar;
        }
        else {  // other has at least 2 variables & this has at least 1 variable
            auto& last = expr.m_firstVar;
            while (last->m_nextVar != nullptr) {
                last = last->m_nextVar;
            }
            last->m_nextVar = other.m_firstVar;
        }
        for (auto var = other.m_firstVar; var; var = var->m_nextVar) {
            var->m_coeff *= -1.0f;
        }
    }
    return expr;
}



template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>
LQMetricExpr<TQuark, TUnit>::
operator*(TUnit coeff) const
{
    LQMetricExpr<TQuark, TUnit> expr(m_firstVar, m_constant*coeff);
    for (auto var = expr.m_firstVar; var; var = var->m_nextVar) {
        var->m_coeff *= coeff;
    }
    return expr;
}

template<class TQuark, class TUnit>
LQMetricExpr<TQuark, TUnit>
LQMetricExpr<TQuark, TUnit>::
operator/(TUnit coeff) const
{
    LQMetricExpr<TQuark, TUnit> expr(*this);
    if (coeff) {
        expr.m_constant /= coeff;
        for (auto var = expr.m_firstVar; var; var = var->m_nextVar) {
            var->m_coeff /= coeff;
        }
    }
    return expr;
}
