#pragma once

#include <forward_list>
#include <glad/glad.h>
#include "LQDeclaration.hpp"

template<class TQuark, class TUnit=GLfloat>
class LQMetric;

#include "LQMetricExpr.hpp"

template<class TQuark, class TUnit>
class LQMetric {
public:
    LQMetric();
    LQMetric(LQMetric<TQuark, TUnit> const& other) = default;
    LQMetric(LQMetricKind kind);
    LQMetric(
        TQuark* quark,
        LQGetterPtr<TQuark, TUnit> getter=nullptr,
        LQSetterPtr<TQuark, TUnit> setter=nullptr);

    void linkToQuark(
        TQuark* quark,
        LQGetterPtr<TQuark, TUnit> getter,
        LQSetterPtr<TQuark, TUnit> setter);

    void recalculate();

    // LQMetric<TQuark, TUnit>& operator=(TUnit unit);
    // LQMetric<TQuark, TUnit>& operator=(LQMetric<TQuark, TUnit>& length);
    // LQMetric<TQuark, TUnit>& operator=(LQMetric<TQuark, TUnit>& length);
    // LQMetric<TQuark, TUnit>& operator=(LQMetric<TQuark, TUnit> length);
    // LQMetric<TQuark, TUnit>& operator=(LQMetric<TQuark, TUnit>&& length) = default;
    // LQMetric<TQuark, TUnit>& operator=(LQMetric<TQuark, TUnit>&& length);

    // LQMetric<TQuark, TUnit> operator+(TUnit variable) const;
    // LQMetric<TQuark, TUnit> operator-(TUnit variable) const;
    // LQMetric<TQuark, TUnit> operator*(TUnit coefficient) const;
    // LQMetric<TQuark, TUnit>&& operator*(TUnit coefficient) &&;
    // LQMetric<TQuark, TUnit> operator/(TUnit coefficient) const;

    // operator TUnit() const;
    operator LQMetricExpr<TQuark, TUnit>();

protected:
    // void appendChild(LQMetric<TQuark, TUnit>* child);
    // void detach();
    // void resizeCallback();

    TQuark* m_quark;
    LQGetterPtr<TQuark, TUnit> m_getter;
    LQSetterPtr<TQuark, TUnit> m_setter;

    LQMetricKind m_kind;

    LQMetricExpr<TQuark, TUnit> m_expr;
    std::forward_list<LQMetric*> m_refs;

    friend class LQMetricExpr<TQuark, TUnit>;
    friend class LQMetricVar<TQuark, TUnit>;
    // LQMetric<TQuark, TUnit>* m_parent;
    // LQMetric<TQuark, TUnit>* m_firstChild;
    // LQMetric<TQuark, TUnit>* m_lastChild;
    // LQMetric<TQuark, TUnit>* m_prevSibling;
    // LQMetric<TQuark, TUnit>* m_nextSibling;
};

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator+(TUnit unit, LQMetric<TQuark, TUnit> length);

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator-(TUnit unit, LQMetric<TQuark, TUnit> length);

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator*(TUnit unit, LQMetric<TQuark, TUnit> length);

// template<class TQuark, class TUnit>
// LQMetric<TQuark, TUnit> operator/(TUnit unit, LQMetric<TQuark, TUnit> length);

#include "LQMetric.ipp"
