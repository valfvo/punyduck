#pragma once

#include <forward_list>
#include <glad/glad.h>
#include "LQDeclaration.hpp"

class LQNumber;

#include "LQuark.hpp"
#include "LQMathExpr.hpp"

template<class T, void (T::*callback)()>
void lqInvokeCallback(void* t)
{
    (static_cast<T*>(t)->*callback)();
}

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

    template<class TQuark, void (TQuark::*callback)()>
    void linkQuark(TQuark& quark);

    void recalc();

    float f() const;
    operator float() const;

    static float old;

protected:
    LQuark* m_quark;
    void (*m_invoke)(void*);

    float m_value;
    LQNumber::Kind m_kind;

    LQMathExpr m_expr;
    std::forward_list<LQNumber*> m_refs;

    friend class LQMathExpr;
    friend class LQMathVar;
};

template<class TQuark, void (TQuark::*callback)()>
void LQNumber::linkQuark(TQuark& quark) {
    m_quark = static_cast<LQuark*>(&quark);
    m_invoke = &lqInvokeCallback<TQuark, callback>;
}
