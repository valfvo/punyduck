#pragma once

#include "LQuark.hpp"
#include "LQTexture.hpp"
#include "LQColor.hpp"
#include "LQShader.hpp"
#include "LQNumber.hpp"

class LQSurface : public LQuark, public LQTexture {
public:
    //constructeurs
    LQSurface();
    LQSurface(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height);
    LQSurface(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height, GLint color);
    LQSurface(LQSurface&& other);

    //accesseurs
    LQMathExpr x();
    LQMathExpr left();
    LQMathExpr right();

    LQMathExpr y();
    LQMathExpr top();
    LQMathExpr bottom();

    LQMathExpr width();
    LQMathExpr height();

    GLuint getVAO() const;
    LQShader* getShader();
    void setShader(LQShader* shader);
    void setClearColor(LQColor const& color);
    void setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);
    void setClearColor(GLint r, GLint g, GLint b, GLint a=255);

    //méthodes
    virtual void drawChildren();
    // void blit(LQTexture const& texture, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    void blit(LQTexture const& texture, GLfloat x, GLfloat y, GLuint VAO);
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
    // void resize(LQNumber&& width, LQNumber&& height);
    // void resizeWidth(LQNumber&& width);
    // void resizeHeight(LQNumber&& height);
    // void scale(GLfloat scaleW, GLfloat scaleH);
    // void scaleWidth(GLfloat scaleW);
    // void scaleHeight(GLfloat scaleH);
    void resizeCallback();
    // void rotate(GLfloat angle);
    // void flip(bool xbool, bool ybool);

protected:
    void linkFramebuffer();
    void linkMetrics();

    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_FBO;
    LQShader* m_shader;
    LQNumber m_x;
    LQNumber m_y;
    LQNumber m_width;
    LQNumber m_height;
    LQColor m_clearColor;

    static GLfloat s_vertices[54];
    static LQShader* s_default_shader;
};
