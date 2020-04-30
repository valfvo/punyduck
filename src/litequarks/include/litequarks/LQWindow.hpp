#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "LQViewable.hpp"

class LQWindow : public LQViewable {
public:
    LQWindow(int width, int height, const char* title);
    void run();
    void update();
    bool alive() const;

    void onEvent();

    GLFWwindow* getGLFWwindow();

private:
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    GLFWwindow* m_window;

    friend void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height);
};
