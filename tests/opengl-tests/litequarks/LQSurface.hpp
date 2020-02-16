#pragma once

#include <cfloat>

#include <glad/glad.h>

class LQTexture;

class LQTexture {
public:
    LQTexture(
        GLfloat width, GLfloat height
        // GLfloat minWidth=0, GLfloat minHeight=0,
        // GLfloat maxWidth=FLT_MAX, GLfloat maxHeight=FLT_MAX
    );
    ~LQTexture();
    void resize(GLfloat width, GLfloat height);

private:
    GLfloat m_width;
    // GLfloat m_minWidth;
    // GLfloat m_maxWidth;
    GLfloat m_height;
    // GLfloat m_minHeight;
    // GLfloat m_maxHeight;
};
