#include <litequarks/LQSurface.hpp>

void LQSurface::move(GLfloat x, GLfloat y) {
    m_x += x;
    m_y += y;
}

void LQSurface::move(glm::vec2 distance) {
    m_x += distance.x;
    m_y += distance.y;
}

void LQSurface::moveX(GLfloat x) {
    m_x += x;
}

void LQSurface::moveY(GLfloat y) {
    m_y += y;
}

void LQSurface::moveTo(GLfloat x, GLfloat y) {
    m_x = x;
    m_y = y;
}

void LQSurface::moveTo(glm::vec2 position) {
    m_x = position.x;
    m_y = position.y;
}

void LQSurface::moveToX(GLfloat x) {
    m_x = x;
}

void LQSurface::moveToY(GLfloat y) {
    m_y = y;
}
