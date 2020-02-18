#include <iostream>
#include "LQTexture.hpp"

GLfloat LQTexture::s_vertexPos[18] = {
//  x     y     z
    0.0f, 1.0f, 0.0f,  // bottom left
    1.0f, 0.0f, 0.0f,  // top right
    0.0f, 0.0f, 0.0f,  // top left

    0.0f, 1.0f, 0.0f,  // bottom left
    1.0f, 1.0f, 0.0f,  // bottom right
    1.0f, 0.0f, 0.0f   // top right
};

GLfloat LQTexture::s_vertexTexPos[12] = {
//  x     y
    0.0f, 1.0f,  // bottom left
    1.0f, 0.0f,  // top right
    0.0f, 0.0f,  // top left

    0.0f, 1.0f,  // bottom left
    1.0f, 1.0f,  // bottom right
    1.0f, 0.0f   // top right
};

GLfloat LQTexture::s_vertexColor[24] = {
//  r     g     b     a
    1.0f, 1.0f, 1.0f, 1.0f,  // bottom left
    1.0f, 1.0f, 1.0f, 1.0f,  // top right
    1.0f, 1.0f, 1.0f, 1.0f,  // top left

    1.0f, 1.0f, 1.0f, 1.0f,  // bottom left
    1.0f, 1.0f, 1.0f, 1.0f,  // bottom right
    1.0f, 1.0f, 1.0f, 1.0f   // top right
};

GLuint LQTexture::s_posVBO = 0;
GLuint LQTexture::s_texPosVBO = 0;
GLuint LQTexture::s_colorVBO = 0;

LQTexture::LQTexture(GLfloat width, GLfloat height)
: m_width(width), m_height(height), m_VAO(0), m_texture(0), m_FBO(0)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, s_posVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0)
    );
    glBindBuffer(GL_ARRAY_BUFFER, s_texPosVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid*>(0)
    );
    glBindBuffer(GL_ARRAY_BUFFER, s_colorVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(0)
    );

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &m_FBO);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void LQTexture::fill(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    // glViewport(0, 0, 50, 50);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    // glActiveTexture(GL_TEXTURE0);
    // glDrawBuffer(GL_COLOR_ATTACHMENT0);
    // glBindTexture(GL_TEXTURE_2D, m_texture);
    // static const float red[] = { 1, 0, 0, 1 };
    // glClearBufferfv(GL_COLOR, 0, red);
    // glClearColor(r, g, b, a);
    // glClear(GL_COLOR_BUFFER_BIT);
    // GLubyte pixels[50*50*4];
    // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    // for (auto& b : pixels) {
    //     b = 125;
    // }
    // glTexImage2D(
    //     GL_TEXTURE_2D, 0, GL_RGBA, 50, 50, 0,
    //     GL_RGBA, GL_UNSIGNED_BYTE, pixels
    // );
    // GLubyte img[50*50*4];
    // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    // for (int x = 0; x < 20; ++x) {
    //     // size_t row = y * elmes_per_line;
    //     size_t col = x * 4;
    //     r = img[0 + col];
    //     g = img[0 + col + 1];
    //     b = img[0 + col + 2]; 
    //     a = img[0 + col + 3];
    //     std::cout << r << ' ' << g << ' ' << b << ' ' << a << std::endl;
    // }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // glViewport(0, 0, 800, 600);
}

void LQTexture::initDefaultVBOs() {
    glGenBuffers(1, &s_posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_posVBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(s_vertexPos), s_vertexPos, GL_STATIC_DRAW
    );
    glGenBuffers(1, &s_texPosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_texPosVBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(s_vertexTexPos), s_vertexTexPos, GL_STATIC_DRAW
    );
    glGenBuffers(1, &s_colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_colorVBO);
    glBufferData(
       GL_ARRAY_BUFFER, sizeof(s_vertexColor), s_vertexColor, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};
