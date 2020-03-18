#pragma once

#include "LQDeclaration.hpp"

template<class TQuark, class TUnit=float>
class LQMetricExpr;

#include "LQMetricVar.hpp"

template<class TQuark, class TUnit>
class LQMetricExpr {
public:
    LQMetricExpr(LQMetric<TQuark, TUnit>* metric);
    LQMetricExpr(LQMetricVar<TQuark, TUnit>* firstVar, TUnit constant);
    LQMetricExpr(const LQMetricExpr<TQuark, TUnit>& other) = default;

    TUnit
    evaluate() const;

    void
    clear();

    LQMetricExpr<TQuark, TUnit>&
    operator=(TUnit constant);

    LQMetricExpr<TQuark, TUnit>&
    operator=(const LQMetricExpr<TQuark, TUnit>& expr);

    // LQMetricExpr<TQuark, TUnit>&
    // operator-=(const LQMetricExpr<TQuark, TUnit>& other);

    LQMetricExpr<TQuark, TUnit>
    operator+(TUnit constant) const;

    LQMetricExpr<TQuark, TUnit>
    operator+(const LQMetricExpr<TQuark, TUnit>& expr) const;

    LQMetricExpr<TQuark, TUnit>
    operator-(TUnit constant) const;

    LQMetricExpr<TQuark, TUnit>
    operator-(const LQMetricExpr<TQuark, TUnit>& expr) const;

    LQMetricExpr<TQuark, TUnit>
    operator*(TUnit coeff) const;

    LQMetricExpr<TQuark, TUnit>
    operator/(TUnit coeff) const;

protected:
    // if constant 0.0f not evaluate (pour passer de l'autre cote du =)
    // si * 0.0f on remove toutes les vars
    LQMetricVar<TQuark, TUnit>* m_firstVar;
    TUnit m_constant;
};

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit> operator+(TUnit unit, LQMetricExpr<TQuark, TUnit> length);

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit> operator-(TUnit unit, LQMetricExpr<TQuark, TUnit> length);

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit> operator*(TUnit unit, LQMetricExpr<TQuark, TUnit> length);

// template<class TQuark, class TUnit>
// LQMetricExpr<TQuark, TUnit> operator/(TUnit unit, LQMetricExpr<TQuark, TUnit> length);

#include "LQMetricExpr.ipp"
