#pragma once

#include "LQMetric.hpp"

template<class TQuark, class TUnit=float>
class LQMetricVar {
public:
    LQMetricVar(LQMetric<TQuark, TUnit>* metric,
                TUnit coeff=1.0f,
                LQMetricVar<TQuark, TUnit>* nextVar=nullptr);
    LQMetricVar(const LQMetricVar<TQuark, TUnit>& other) = default;

    void
    setNextVar(LQMetricVar<TQuark, TUnit>* nextVar);

    TUnit
    evaluate() const;

    bool
    validWith(const LQMetric<TQuark, TUnit>& metric) const;


    // LQMetricExpr<TQuark, TUnit> operator+(TUnit constant) const;
    // LQMetricExpr<TQuark, TUnit> operator+(LQMetric<TQuark, TUnit> metric) const;
    // LQMetricExpr<TQuark, TUnit> operator+(LQMetricVar<TQuark, TUnit> var) const;
    // LQMetricExpr<TQuark, TUnit> operator+(LQMetricExpr<TQuark, TUnit> expr) const;

    // LQMetricExpr<TQuark, TUnit> operator-(TUnit constant) const;
    // LQMetricExpr<TQuark, TUnit> operator-(LQMetric<TQuark, TUnit> metric) const;
    // LQMetricExpr<TQuark, TUnit> operator-(LQMetricVar<TQuark, TUnit> var) const;
    // LQMetricExpr<TQuark, TUnit> operator-(LQMetricExpr<TQuark, TUnit> expr) const;

    // LQMetricVar<TQuark, TUnit> operator*(TUnit coeff) const;
    // LQMetricVar<TQuark, TUnit> operator/(TUnit coeff) const;

protected:
    LQMetric<TQuark, TUnit>* m_metric;
    TUnit m_coeff;
    LQMetricVar<TQuark, TUnit>* m_nextVar;

    friend class LQMetricExpr<TQuark, TUnit>;
};

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// operator+(TUnit coeff, const LQMetricVar<TQuark, TUnit>& var);

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit>
// operator-(TUnit coeff, const LQMetricVar<TQuark, TUnit>& var);

// template<class TQuark, class TUnit>
// LQMetricVar<TQuark, TUnit>
// operator*(TUnit coeff, const LQMetricVar<TQuark, TUnit>& var);

#include "LQMetricVar.ipp"
