#include <litequarks/LQSurface.hpp>

void LQSurface::resize(GLfloat width, GLfloat height) {
    resizeWidth(width);
    resizeHeight(height);
}

void LQSurface::resizeWidth(GLfloat width) {
    m_width = width;
}

void LQSurface::resizeHeight(GLfloat height) {
    m_height = height;
}

void LQSurface::scale(GLfloat scaleW, GLfloat scaleH) {
    scaleWidth(scaleW);
    scaleHeight(scaleH);
}

void LQSurface::scaleWidth(GLfloat scaleW) {
    m_width *= scaleW;
}

void LQSurface::scaleHeight(GLfloat scaleH) {
    m_height *= scaleH;
}
