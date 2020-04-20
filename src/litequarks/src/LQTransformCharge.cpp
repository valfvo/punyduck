#include <litequarks/LQSurface.hpp>

void LQSurface::resize(GLfloat width, GLfloat height) {
    resizeWidth(width);
    resizeHeight(height);
}

void LQSurface::resizeWidth(GLfloat _width) {
    width() = _width;
}

void LQSurface::resizeHeight(GLfloat _height) {
    height() = _height;
}

void LQSurface::scale(GLfloat scaleW, GLfloat scaleH) {
    scaleWidth(scaleW);
    scaleHeight(scaleH);
}

void LQSurface::scaleWidth(GLfloat scaleW) {
    width() *= scaleW;
}

void LQSurface::scaleHeight(GLfloat scaleH) {
    height() *= scaleH;
}
