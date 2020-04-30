#include <iostream>

#include <litequarks/LQAppController.hpp>
#include <litequarks/LQWindow.hpp>

void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height);

LQWindow::LQWindow(int _width, int _height, char const* title)
: LQViewable(), m_window(nullptr)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    m_window = glfwCreateWindow(_width, _height, title, nullptr, nullptr);

    if (m_window == nullptr) {
        std::clog << "failed to create a window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, LQWindowFBSizeCallback);
    glfwSetCursorPosCallback(m_window, LQAppController::cursor_position_callback);
    glfwSetMouseButtonCallback(m_window, LQAppController::mouse_button_callback);
    glfwSetKeyCallback(m_window, LQAppController::key_callback);
    glfwSetCharCallback(m_window, LQAppController::character_callback);
    glfwSetScrollCallback(m_window, LQAppController::scroll_callback);
    glfwSetDropCallback(m_window, LQAppController::drop_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::clog << "failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                        GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_shader = new LQShader("shaders/shaderVertex.txt", "shaders/shaderFragment.txt");

    m_width.linkQuark<LQWindow>(*this);
    m_height.linkQuark<LQWindow>(*this);
    width() = m_texWidth = _width;
    height() = m_texHeight =_height;

    LQAppController::setWindow(this);
    lqOn<LQClickEvent>(this, onEvent);
    lqOn<LQFocusGainEvent>(this, onEvent);
    lqOn<LQScrollEvent>(this, onEvent);
    lqOn<LQDropEvent>(this, onEvent);
}

void LQWindow::run() {
    while (alive()) {
        clear();
        drawChildren();
        update();
    }
}

void LQWindow::update() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    LQAppController::pollResponses();
    LQAppController::pollEvents();
    processInput(m_window);
}

bool LQWindow::alive() const {
    return !glfwWindowShouldClose(m_window);
}

void LQWindow::onEvent() { }

GLFWwindow* LQWindow::getGLFWwindow() {
    return m_window;
}

void LQWindow::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void LQWindow::framebufferSizeCallback(GLFWwindow* window, int _width, int _height) {
    width() = _width;
    height() = _height;
    LQAppController::resetMousePosition();
}

void LQWindowFBSizeCallback(GLFWwindow* window, int width, int height) {
    auto lqWindow = static_cast<LQWindow*>(glfwGetWindowUserPointer(window));
    lqWindow->framebufferSizeCallback(window, width, height);
}
    