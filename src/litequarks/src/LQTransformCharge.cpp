#include <litequarks/LQSurface.hpp>

// void LQSurface::resize(LQNumber&& width, LQNumber&& height) {
//     resizeWidth(std::move(width));
//     resizeHeight(std::move(height));
// }

// void LQSurface::resizeWidth(LQNumber&& _width) {
//     width() = std::move(_width);
// }

// void LQSurface::resizeHeight(LQNumber&& _height) {
//     height() = std::move(_height);
// }

// void LQSurface::scale(GLfloat scaleW, GLfloat scaleH) {
//     scaleWidth(scaleW);
//     scaleHeight(scaleH);
// }

// void LQSurface::scaleWidth(GLfloat scaleW) {
//     width() *= scaleW;
// }

// void LQSurface::scaleHeight(GLfloat scaleH) {
//     height() *= scaleH;
// }

void LQSurface::resizeCallback() {
    LQTexture old = resize(m_width.f(), m_height.f());
    linkFramebuffer();
    clear();
    // blit(old, 0.0f, 0.0f, m_VAO);
    glDeleteTextures(1, &old.m_id);
}
