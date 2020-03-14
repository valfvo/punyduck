#include <litequarks/LQColor.hpp>

//constructeurs
LQColor::LQColor()
: m_r(0.0f), m_g(0.0f), m_b(0.0f), m_a(1.0f)
{
}

LQColor::LQColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
: m_r(r), m_g(g), m_b(b), m_a(a)
{
}

LQColor::LQColor(GLint r, GLint g, GLint b, GLint a)
: m_r(r/255.0f), m_g(g/255.0f), m_b(b/255.0f), m_a(a/255.0f)
{
}

LQColor::LQColor(GLint rgb)
: LQColor(rgb>>16 & 0xFF, rgb>>8 & 0xFF, rgb & 0xFF)
{ 
}

//accesseurs
GLfloat LQColor::r() const
{
    return m_r;
}

GLfloat LQColor::g() const
{
    return m_g;
}

GLfloat LQColor::b() const
{
    return m_b;
}

GLfloat LQColor::a() const
{
    return m_a;
}

void LQColor::setR(GLfloat r)
{
    m_r = r;
}

void LQColor::setG(GLfloat g)
{
    m_g = g;
}

void LQColor::setB(GLfloat b)
{
    m_b = b;
}

void LQColor::setA(GLfloat a)
{
    m_a = a;
}
