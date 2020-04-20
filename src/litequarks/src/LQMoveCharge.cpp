#include <litequarks/LQSurface.hpp>
#include <glm/vec2.hpp>

void LQSurface::move(GLfloat _x, GLfloat _y) {
    x() += _x;
    y() += _y;
}

void LQSurface::move(glm::vec2 distance) {
    x() += distance.x;
    y() += distance.y;
}

void LQSurface::moveX(GLfloat _x) {
    x() += _x;
}

void LQSurface::moveY(GLfloat _y) {
    y() += _y;
}

void LQSurface::moveTo(GLfloat _x, GLfloat _y) {
    x() = _x;
    y() = _y;
}

void LQSurface::moveTo(glm::vec2 position) {
    x() = position.x;
    y() = position.y;
}

void LQSurface::moveToX(GLfloat _x) {
    x() = _x;
}

void LQSurface::moveToY(GLfloat _y) {
    y() = _y;
}
