#pragma once

#include <glad/glad.h>

class LQColor
{
    public :
    //constructeurs
    LQColor();
    LQColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);
    LQColor(GLint r, GLint g, GLint b, GLint a=255);
    LQColor(GLint rgb);

    //accesseurs
    GLfloat r() const;
    GLfloat g() const;
    GLfloat b() const;
    GLfloat a() const;

    void setR(GLfloat r);
    void setG(GLfloat g);
    void setB(GLfloat b);
    void setA(GLfloat a);

    protected : 
    GLfloat m_r;
    GLfloat m_g;
    GLfloat m_b;
    GLfloat m_a;
};