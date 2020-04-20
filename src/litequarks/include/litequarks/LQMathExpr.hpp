#pragma once

#include "LQDeclaration.hpp"

class LQMathExpr;

#include "LQMathVar.hpp"

class LQMathExpr {
public:
    LQMathExpr();
    LQMathExpr(LQNumber& number);
    LQMathExpr(LQMathExpr&& other);
    LQMathExpr(const LQMathExpr& other) = delete;
    ~LQMathExpr();

    float eval() const;
    void reset();

    LQMathExpr& operator=(float constant);
    LQMathExpr& operator=(LQMathExpr&& expr);

    LQMathExpr& operator+=(float constant);
    LQMathExpr& operator-=(float constant);

    LQMathExpr& operator*=(float coeff);
    LQMathExpr& operator/=(float coeff);

    LQMathExpr&& operator+(float constant) &&;
    LQMathExpr&& operator+(LQMathExpr&& other) &&;

    LQMathExpr&& operator-(float constant) &&;
    LQMathExpr&& operator-(LQMathExpr&& other) &&;

    LQMathExpr&& operator*(float coeff) &&;
    LQMathExpr&& operator/(float coeff) &&;

protected:
    void addCompatible(LQMathVar* first, float coeff=1.0f);

    LQMathVar* m_first;
    LQMathVar* m_last;
    float m_constant;
};

LQMathExpr&& operator+(float constant, LQMathExpr&& expr);
LQMathExpr&& operator-(float constant, LQMathExpr&& expr);

LQMathExpr&& operator*(float coeff, LQMathExpr&& expr);
LQMathExpr&& operator/(float coeff, LQMathExpr&& expr);
