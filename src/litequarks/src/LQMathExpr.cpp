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

float LQMathExpr::eval() const {
    float value = m_constant;
    for (auto var = m_first; var; var = var->m_next) {
        value += var->eval();
    }
    return value;
}

LQMathExpr::~LQMathExpr() {
    reset();
}

void LQMathExpr::reset() {
    m_constant = 0.0f;
    auto prev = m_first;
    if (prev) {
        for (auto* p_var = prev->m_next; p_var; p_var = p_var->m_next) {
            auto& refs = p_var->m_number->m_refs;
            auto prev_it = refs.before_begin();
            for (auto it = refs.begin(); it != refs.end(); ++it) {
                if (*it == m_first->m_number) {
                    refs.erase_after(prev_it);
                    break;
                }
                prev_it = it++;
            }
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

    for (auto* p_var = first->m_next; p_var; p_var = p_var->m_next) {
        if (first->compatible(*p_number)) {
            first->m_coeff *= coeff;
            *pp_var = first;
            pp_var = &(*pp_var)->m_next;
            first->m_number->m_refs.push_front(p_number);
        }
        else {
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
