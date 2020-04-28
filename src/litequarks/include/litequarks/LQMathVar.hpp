#pragma once

class LQMathVar;
class LQNumber;

class LQMathVar {
public:
    LQMathVar(LQNumber& number, float coeff=1.0f);
    LQMathVar(const LQMathVar& other) = delete;

    void setNext(LQMathVar* next);

    float eval() const;

    bool parentCoords(const LQNumber& number) const;
    bool compatible(const LQNumber& number) const;

protected:
    LQNumber* m_number;
    float m_coeff;
    LQMathVar* m_next;

    friend class LQNumber;
    friend class LQMathExpr;
};
