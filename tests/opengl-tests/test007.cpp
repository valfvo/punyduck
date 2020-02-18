#include <iostream>

#include "litequarks/LQWindow.hpp"
#include "litequarks/LQTexture.hpp"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

int main() {
    glfwInit();
    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    LQTexture::initDefaultVBOs();

    LQTexture tex(50, 50);
    tex.fill(1.0f, 1.0f, 1.0f);

    while (window.alive()) {
        window.clear();
        window.blit(tex, 50, 50);
        window.update();
    }

    return EXIT_SUCCESS;
}
