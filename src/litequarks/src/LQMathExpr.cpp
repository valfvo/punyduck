#include <utility>  // std::move

#include <litequarks/LQMathExpr.hpp>
#include <litequarks/LQNumber.hpp>

LQMathExpr::LQMathExpr()
: m_first(nullptr), m_last(nullptr), m_constant(0.0f) { }

LQMathExpr::LQMathExpr(LQNumber& number)
: m_first(new LQMathVar(number)), m_last(nullptr), m_constant(0.0f)
{
    m_last = m_first;
}

LQMathExpr::LQMathExpr(LQMathExpr&& other)
: m_first(other.m_first), m_last(other.m_last), m_constant(other.m_constant)
{
    other.m_first = other.m_last = nullptr;
}

LQMathExpr::~LQMathExpr() {
    reset();
}

float LQMathExpr::eval() const {
    float value = m_constant;
    for (auto var = m_first; var; var = var->m_next) {
        value += var->eval();
    }
    return value;
}

void LQMathExpr::reset() {
    m_constant = 0.0f;
    auto* prev = m_first;
    if (prev) {
        for (auto* p_var = prev->m_next; p_var; p_var = p_var->m_next) {
            prev->m_number->removeRef(m_first->m_number);
            delete prev;
            prev = p_var;
        }
        if (prev) {
            prev->m_number->removeRef(m_first->m_number);
            delete prev;
        }
        m_first = m_last = nullptr;
    }
}

LQMathExpr& LQMathExpr::operator=(float constant) {
    if (m_first) {
        m_first->m_number->m_expr.reset();
        m_first->m_number->m_expr.m_constant = constant - m_constant;
        addCompatible(m_first->m_next, -1.0f);
        m_first->m_number->recalc();
    }
    return *this;
}

LQMathExpr& LQMathExpr::operator=(LQMathExpr&& other) {
    if (m_first) {
        m_first->m_number->m_expr.reset();
        m_first->m_number->m_expr.m_constant = other.m_constant - m_constant;
        addCompatible(other.m_first);
        other.m_first = other.m_last = nullptr;
        addCompatible(m_first->m_next, -1.0f);
        m_first->m_number->recalc();
    }
    return *this;
}

LQMathExpr& LQMathExpr::operator+=(float constant) {
    m_constant += constant;
    return *this;
}

LQMathExpr& LQMathExpr::operator-=(float constant) {
    m_constant -= constant;
    return *this;
}

LQMathExpr& LQMathExpr::operator*=(float coeff) {
    if (coeff) {
        m_constant *= coeff;
        for (auto var = m_first; var; var = var->m_next) {
            var->m_coeff *= coeff;
        }
    }
    else {
        reset();
    }
    return *this;
}

LQMathExpr& LQMathExpr::operator/=(float coeff) {
    if (coeff) {
        m_constant /= coeff;
        for (auto var = m_first; var; var = var->m_next) {
            var->m_coeff /= coeff;
        }
    }
    return *this;
}

LQMathExpr&& LQMathExpr::operator+(float constant) && {
    m_constant += constant;
    return std::move(*this);
}

LQMathExpr&& LQMathExpr::operator+(LQMathExpr&& other) && {
    m_constant += other.m_constant;
    if (m_last) {
        m_last->m_next = other.m_first;
        m_last = other.m_last;
    }
    else {
        m_first = m_last = other.m_first;
    }
    other.m_first = other.m_last = nullptr;
    return std::move(*this);
}

LQMathExpr&& LQMathExpr::operator-(float constant) && {
    m_constant -= constant;
    return std::move(*this);
}

LQMathExpr&& LQMathExpr::operator-(LQMathExpr&& other) && {
    m_constant -= other.m_constant;
    if (m_last) {
        m_last->m_next = other.m_first;
        m_last = other.m_last;
    }
    else {
        m_first = m_last = other.m_first;
    }
    for (auto var = other.m_first; var; var = var->m_next) {
        var->m_coeff *= -1.0f;
    }
    other.m_first = other.m_last = nullptr;
    return std::move(*this);
}

LQMathExpr&& LQMathExpr::operator*(float coeff) && {
    *this *= coeff;
    return std::move(*this);
}

LQMathExpr&& LQMathExpr::operator/(float coeff) && {
    *this /= coeff;
    return std::move(*this);
}

void LQMathExpr::addCompatible(LQMathVar* first, float coeff) {
    if (!m_first || !first) {
        return;
    }
    coeff /= m_first->m_coeff;
    auto* p_number = m_first->m_number;
    auto** pp_var = p_number->m_expr.m_first ?
        &p_number->m_expr.m_last :
        &p_number->m_expr.m_first;

    auto* p_var = first;
    while (p_var) {
        if (p_var->compatible(*p_number)) {
            p_var->m_coeff *= coeff;
            *pp_var = p_var;
            pp_var = &(*pp_var)->m_next;
            p_var->m_number->m_refs.push_front(p_number);
            p_var = p_var->m_next;
        }
        else {
            first = p_var;
            p_var = p_var->m_next;
            delete first;
        }
    }
    p_number->m_expr.m_last = *pp_var;
}

LQMathExpr&& operator+(float constant, LQMathExpr&& expr) {
    expr += constant;
    return std::move(expr);
}

LQMathExpr&& operator-(float constant, LQMathExpr&& expr) {
    expr -= constant;
    return std::move(expr);
}

LQMathExpr&& operator*(float coeff, LQMathExpr&& expr) {
    expr *= coeff;
    return std::move(expr);
}

LQMathExpr&& operator/(float coeff, LQMathExpr&& expr) {
    expr /= coeff;
    return std::move(expr);
}
