#pragma once

#include <glad/glad.h>

class LQTexture;

class LQTexture {
public:
    LQTexture(GLfloat width, GLfloat height);
    // ~LQTexture();
    // void resize(GLfloat width, GLfloat height);
    // setOpacity
    void fill(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);
    static void initDefaultVBOs();

private:
    GLfloat m_width;
    GLfloat m_height;
    GLuint  m_VAO;
    GLuint  m_texture;
    GLuint  m_FBO;
    // resizeColor
    // backgroundColor

    static GLfloat s_vertexPos[18];
    static GLfloat s_vertexTexPos[12];
    static GLfloat s_vertexColor[24];
    static GLuint  s_posVBO;
    static GLuint  s_texPosVBO;
    static GLuint  s_colorVBO;

    friend class LQWindow;
};
