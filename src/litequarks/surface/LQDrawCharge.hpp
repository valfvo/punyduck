#pragma once

#include "LQBasicCharge.hpp"
#include "LQSurface.hpp"

typedef int LQEnum;
#define LQ_BORDER_CENTER  0
#define LQ_BORDER_INNER   1
#define LQ_BORDER_BOTTOM  2
#define LQ_BORDER_OUTER   3
#define LQ_BORDER_TOP     4

class LQDrawCharge : public LQBasicCharge<LQSurface>
{
    public :
    void rect(GLfloat width, GLfloat height, GLfloat x, GLfloat y);
    void circle(GLfloat radius, GLfloat centerX, GLfloat centerY);
    void square(GLfloat width, GLfloat x, GLfloat y);
    void line(GLfloat x, GLfloat y, GLfloat thickness, LQEnum border=LQ_BORDER_CENTER);
    void triangle(GLfloat a, GLfloat b, GLfloat c);
    void ellipse(GLfloat radius, GLfloat height, GLfloat centerX, GLfloat centerY);
    void arc(GLfloat radius, GLfloat startAngle, GLfloat angle);
};