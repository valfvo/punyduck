#include <iostream>

#include "LQWindow.hpp"

void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height);

LQWindow::LQWindow(int width, int height, char const* title)
: m_width(width), m_height(height), m_window(nullptr),
  m_shader()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (m_window == nullptr) {
        std::clog << "failed to create a window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, LQWindowFBSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::clog << "failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    m_shader.init("transform.vert", "transform.frag");
}

void LQWindow::clear() {
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void LQWindow::update() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    processInput(m_window);
}

bool LQWindow::alive() const {
    return !glfwWindowShouldClose(m_window);
}

void LQWindow::blit(LQTexture const& texture, GLfloat x, GLfloat y) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(texture.m_width, texture.m_height, 1.0f));
    
    m_shader.set("projection", glm::ortho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f));
    m_shader.set("model", model);
    m_shader.set("texture0", 0);
    m_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.m_texture);
    glBindVertexArray(texture.m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void LQWindow::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void LQWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}

void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height) {
    auto lqWindow = static_cast<LQWindow*>(glfwGetWindowUserPointer(window));
    lqWindow->framebufferSizeCallback(window, width, height);
}
