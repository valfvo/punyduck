#pragma once

#include <vector>
#include <glad/glad.h>
#include "LQDeclaration.hpp"

template<class TQuark, class TUnit=GLfloat>
class LQLength {
public:
    LQLength();
    LQLength(LQLength<TQuark, TUnit> const& other) = default;
    LQLength(
        TQuark* quark,
        LQGetterPointer<TQuark, TUnit> lengthGetter=nullptr,
        LQSetterPointer<TQuark, TUnit> lengthSetter=nullptr,
        TUnit coefficient=1.0f,
        TUnit variable=0.0f);

    void linkToQuark(
        TQuark* quark,
        LQGetterPointer<TQuark, TUnit> lengthGetter,
        LQSetterPointer<TQuark, TUnit> lengthSetter);

    LQLength<TQuark, TUnit> mock();

    LQLength<TQuark, TUnit>& operator=(TUnit unit);
    // LQLength<TQuark, TUnit>& operator=(LQLength<TQuark, TUnit>& length);
    // LQLength<TQuark, TUnit>& operator=(LQLength<TQuark, TUnit>& length);
    LQLength<TQuark, TUnit>& operator=(LQLength<TQuark, TUnit> length);
    // LQLength<TQuark, TUnit>& operator=(LQLength<TQuark, TUnit>&& length) = default;
    // LQLength<TQuark, TUnit>& operator=(LQLength<TQuark, TUnit>&& length);

    LQLength<TQuark, TUnit> operator+(TUnit variable) const;
    LQLength<TQuark, TUnit> operator-(TUnit variable) const;
    LQLength<TQuark, TUnit> operator*(TUnit coefficient) const;
    // LQLength<TQuark, TUnit>&& operator*(TUnit coefficient) &&;
    LQLength<TQuark, TUnit> operator/(TUnit coefficient) const;

    operator TUnit() const;

protected:
    LQLength(
        TQuark* quark,
        LQGetterPointer<TQuark, TUnit> lengthGetter,
        LQSetterPointer<TQuark, TUnit> lengthSetter,
        TUnit coefficient,
        TUnit variable,
        LQLength<TQuark, TUnit>* parent,
        LQLength<TQuark, TUnit>* firstChild,
        LQLength<TQuark, TUnit>* lastChild,
        LQLength<TQuark, TUnit>* prevSibling,
        LQLength<TQuark, TUnit>* nextSibling);

    void appendChild(LQLength<TQuark, TUnit>* child);
    void detach();
    void resizeCallback();

    TQuark* m_quark;
    LQGetterPointer<TQuark, TUnit> m_lengthGetter;
    LQSetterPointer<TQuark, TUnit> m_lengthSetter;

    TUnit m_coefficient;
    TUnit m_variable;

    LQLength<TQuark, TUnit>* m_parent;
    LQLength<TQuark, TUnit>* m_firstChild;
    LQLength<TQuark, TUnit>* m_lastChild;
    LQLength<TQuark, TUnit>* m_prevSibling;
    LQLength<TQuark, TUnit>* m_nextSibling;
};

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator+(TUnit unit, LQLength<TQuark, TUnit> length);

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator-(TUnit unit, LQLength<TQuark, TUnit> length);

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator*(TUnit unit, LQLength<TQuark, TUnit> length);

template<class TQuark, class TUnit>
LQLength<TQuark, TUnit> operator/(TUnit unit, LQLength<TQuark, TUnit> length);

#include "LQLength.ipp"
