#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LQTexture.hpp"
#include "../Shader.hpp"

class LQWindow;

class LQWindow {
public:
    LQWindow(int width, int height, char const* title);
    void clear();
    void update();
    bool alive() const;
    void blit(LQTexture const& texture, GLfloat x, GLfloat y);

private:
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);

    GLfloat m_width;
    GLfloat m_height;
    GLFWwindow* m_window;
    Shader m_shader;

    friend void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height);
};
