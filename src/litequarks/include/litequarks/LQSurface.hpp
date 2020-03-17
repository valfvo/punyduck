#pragma once

#include "LQuark.hpp"
#include "LQTexture.hpp"
#include "LQColor.hpp"
#include "LQShader.hpp"

class LQSurface : public LQuark, public LQTexture {
public:
    //constructeurs
    LQSurface();
    LQSurface(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    LQSurface(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLint color);

    //accesseurs
    GLfloat getX() { return m_x; }
    GLfloat getY() { return m_y; }
    GLfloat getWidth() { return m_width; }
    GLfloat getHeight() { return m_height; }

    GLuint getVAO() const;
    LQShader* getShader();
    void setClearColor(LQColor const& color);
    void setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);
    void setClearColor(GLint r, GLint g, GLint b, GLint a=255);

    //méthodes
    void drawChildren();
    // void blit(LQTexture const& texture, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    // void blit(LQTexture const& texture, GLfloat x, GLfloat y);
    void blit(const LQSurface& surface);
    void fill(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);
    void fill(LQColor const& color);
    void clear();

    // LQMoveCharge
    void move(GLfloat x, GLfloat y);
    void move(glm::vec2 distance);
    void moveX(GLfloat x);
    void moveY(GLfloat y);
    void moveTo(GLfloat x, GLfloat y);
    void moveTo(glm::vec2 position);
    void moveToX(GLfloat x);
    void moveToY(GLfloat y);

    // LQDrawCharge

    // LQTransformCharge    
    void resize(GLfloat width, GLfloat height);
    void resizeWidth(GLfloat width);
    void resizeHeight(GLfloat height);
    void scale(GLfloat scaleW, GLfloat scaleH);
    void scaleWidth(GLfloat scaleW);
    void scaleHeight(GLfloat scaleH);
    // void rotate(GLfloat angle);
    // void flip(bool xbool, bool ybool);

protected:
    static GLfloat s_vertices[54];
    static LQShader* s_default_shader;
    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_FBO;
    LQShader* m_shader;
    GLfloat m_x;
    GLfloat m_y;
    GLfloat m_width;
    GLfloat m_height;
    LQTexture* m_shapeMap;
    LQColor m_clearColor;
};