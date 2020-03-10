#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LQSurface.hpp"

class LQWindow : public LQSurface 
{
    public:
    LQWindow(int width, int height, char const* title);
    void update();
    bool alive() const;

    private:
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    GLFWwindow* m_window;

    friend void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height);
};
