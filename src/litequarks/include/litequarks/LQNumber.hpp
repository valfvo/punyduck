#pragma once

#include <forward_list>
#include <glad/glad.h>
#include "LQDeclaration.hpp"

class LQNumber;

#include "LQuark.hpp"
#include "LQMathExpr.hpp"

class LQNumber {
public:
    enum class Kind {
        value,
        length,
        coords
    };

    LQNumber();
    LQNumber(float value);
    LQNumber(LQMathExpr&& expr);
    LQNumber(LQNumber::Kind kind);
    LQNumber(LQNumber&& other);
    LQNumber(const LQNumber& other) = delete;

    template<class TQuark, void (TQuark::*callback)()=nullptr>
    void linkQuark(TQuark& quark);

    void recalc();

    float f() const;
    operator float() const;

    static float old();

protected:
    LQuark* m_quark;
    void (*m_invoke)(LQuark*);

    float m_value;
    LQNumber::Kind m_kind;

    LQMathExpr m_expr;
    std::forward_list<LQNumber*> m_refs;

    static float s_old;

    friend class LQMathExpr;
    friend class LQMathVar;
};

template<class T, void (T::*callback)()>
void lqInvokeNumberCallback(LQuark* t)
{
    (static_cast<T*>(t)->*callback)();
}

template<class TQuark, void (TQuark::*callback)()>
void LQNumber::linkQuark(TQuark& quark) {
    m_quark = static_cast<LQuark*>(&quark);
    m_invoke = callback ? &lqInvokeNumberCallback<TQuark, callback> : nullptr;
}
