#include <cmath>  // std::ceil
#include <iostream>
#include <utility>  // std::move
#include <litequarks/LQSurface.hpp>
#include <glm/gtc/matrix_transform.hpp>

//initialisation attributs static
GLfloat LQSurface::s_vertices[54] = {
//  x     y     z      //texture position       //color
    0.0f, 1.0f, 0.0f,  0.0f, 0.0f,              1.0f, 1.0f, 1.0f, 1.0f,  // bottom left
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f,              1.0f, 1.0f, 1.0f, 1.0f,  // top right
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f,              1.0f, 1.0f, 1.0f, 1.0f,  // top left

    0.0f, 1.0f, 0.0f,  0.0f, 0.0f,              1.0f, 1.0f, 1.0f, 1.0f,  // bottom left
    1.0f, 1.0f, 0.0f,  1.0f, 0.0f,              1.0f, 1.0f, 1.0f, 1.0f,  // bottom right
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f,              1.0f, 1.0f, 1.0f, 1.0f   // top right
};

// Shader* LQSurface::s_default_shader = new Shader("shaderVertex.txt", "shaderFragment.txt");

//constructeurs
LQSurface::LQSurface()
: LQuark(), LQTexture(), m_VBO(0), m_VAO(0), m_FBO(0), m_shader(nullptr),
  m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f), m_clearColor()
{
    linkMetrics();
}

LQSurface::LQSurface(LQNumber&& x, LQNumber&& y,
                     LQNumber&& width, LQNumber&& height)
// : LQTexture(), m_VBO(0), m_VAO(0), m_FBO(0), m_shader(s_default_shader),
: LQuark(), LQTexture(width, height), m_VBO(0), m_VAO(0), m_FBO(0),
  m_shader(new LQShader("shaders/shaderVertex.txt", "shaders/shaderFragment.txt")),
  m_x(std::move(x)), m_y(std::move(y)),
  m_width(std::move(width)), m_height(std::move(height)), m_clearColor()
{
    linkMetrics();
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
    glGenFramebuffers(1, &m_FBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    // color attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));

    linkFramebuffer();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

LQSurface::LQSurface(LQNumber&& _x, LQNumber&& _y,
                     LQNumber&& _width, LQNumber&& _height,
                     GLint color, const std::string& iconPath)
: LQSurface(std::move(_x), std::move(_y), std::move(_width), std::move(_height))
{
    m_clearColor = LQColor(color);
    clear();
    if (!iconPath.empty()) {
        LQTexture icon("images/" + iconPath, m_width.f(), m_height.f());
        blit(icon, 0.0f, 0.0f, m_VAO);
    }
}

LQSurface::LQSurface(LQSurface&& other)
: LQuark(std::move(static_cast<LQuark&>(other))),
  LQTexture(std::move(static_cast<LQTexture&>(other))),
  m_VBO(other.m_VBO), m_VAO(other.m_VAO), m_FBO(other.m_FBO),
  m_shader(other.m_shader),
  m_x(std::move(other.m_x)), m_y(std::move(other.m_y)),
  m_width(std::move(other.m_width)), m_height(std::move(m_height)),
  m_clearColor(other.m_clearColor)
{
    other.m_VBO = other.m_VAO = other.m_FBO = 0;
    other.m_shader = nullptr;
}

float LQSurface::xF() const {
    return m_x.f();
}

float LQSurface::yF() const {
    return m_y.f();
}

float LQSurface::widthF() const {
    return m_width.f();
}

float LQSurface::heightF() const {
    return m_height.f();
}

LQMathExpr LQSurface::x() {
    return m_x;
}

LQMathExpr LQSurface::left() {
    return m_x;
}

LQMathExpr LQSurface::right() {
    return LQMathExpr(m_width) + LQMathExpr(m_x);
}

LQMathExpr LQSurface::y() {
    return m_y;
}

LQMathExpr LQSurface::top() {
    return m_y;
}

LQMathExpr LQSurface::bottom() {
    return LQMathExpr(m_y) + LQMathExpr(m_height);
}

LQMathExpr LQSurface::width() {
    return m_width;
}

LQMathExpr LQSurface::height() {
    return m_height;
}

GLuint LQSurface::getVAO() const
{
    return m_VAO;
}

LQShader* LQSurface::getShader()
{
    return m_shader;
}

void LQSurface::setShader(LQShader* shader) {
    delete m_shader;
    m_shader = shader;
}

void LQSurface::setClearColor(LQColor const& color) {
    m_clearColor = color;
}

void LQSurface::setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    m_clearColor = LQColor(r, g, b, a);
}

void LQSurface::setClearColor(GLint r, GLint g, GLint b, GLint a) {
    m_clearColor = LQColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

// méthodes
// void LQSurface::blit(LQTexture const& texture, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
// {
//     glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
//     model = glm::scale(model, glm::vec3(width, height, 1.0f));

//     m_shader->use();
//     m_shader->set("model", model);
//     m_shader->set("projection", glm::ortho(0.0f, m_width, m_height, 0.0f, -1.0f, 0.0f));
//     m_shader->set("texture0", 0);

//     glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, texture.m_id);
//     glBindVertexArray(m_VAO);
//     glDrawArrays(GL_TRIANGLES, 0, 6);

//     glBindTexture(GL_TEXTURE_2D, 0);
//     glBindVertexArray(0);
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

void LQSurface::draw() {
    clear();
}

void LQSurface::drawChildren() {
    for (
        LQSurface* child = static_cast<LQSurface*>(m_firstChild);
        child != nullptr;
        child = static_cast<LQSurface*>(child->m_nextSibling)
    ) {
        child->drawChildren();
        blit(*child);
    }
}

void LQSurface::blit(const LQTexture& texture, GLfloat x, GLfloat y, GLuint VAO)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(texture.m_texWidth, texture.m_texHeight, 1.0f));

    m_shader->use();
    m_shader->set("model", model);
    m_shader->set("projection",
        glm::ortho(0.0f, m_width.f(), m_height.f(), 0.0f, -1.0f, 0.0f));
    m_shader->set("texture0", 0);

    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.m_id);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LQSurface::blit(LQSurface const& surface) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f),
        glm::vec3(surface.m_x.i(), surface.m_y.i(), 0.0f));
    model = glm::scale(model,
        glm::vec3(surface.m_width.f(), surface.m_height.f(), 1.0f));

    m_shader->use();
    m_shader->set("model", model);
    m_shader->set("projection",
        glm::ortho(0.0f, m_width.f(), m_height.f(), 0.0f, -1.0f, 0.0f));
    m_shader->set("texture0", 0);

    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, surface.m_id);
    glBindVertexArray(surface.m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LQSurface::fill(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);   //bind pour colorer le bon buffer
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   //debind
}

void LQSurface::fill(LQColor const& color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); 
    glClearColor(color.r(), color.g(), color.b(), color.a());
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LQSurface::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); 
    glClearColor(m_clearColor.r(), m_clearColor.g(), m_clearColor.b(), m_clearColor.a());
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LQSurface::linkFramebuffer() {
    if (m_id) {  // width >= 1 && height >= 1 -> texture generated
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0
        );
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete"
                      << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void LQSurface::linkMetrics() {
    m_x.linkQuark<LQSurface, resizeCallback>(*this);
    m_y.linkQuark<LQSurface, resizeCallback>(*this);
    m_width.linkQuark<LQSurface, resizeCallback>(*this);
    m_height.linkQuark<LQSurface, resizeCallback>(*this);
}
