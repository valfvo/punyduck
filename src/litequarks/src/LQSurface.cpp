#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <litequarks/LQSurface.hpp>

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
  m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f),
  m_shapeMap(nullptr), m_clearColor()
{
}

LQSurface::LQSurface(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
// : LQTexture(), m_VBO(0), m_VAO(0), m_FBO(0), m_shader(s_default_shader),
: LQuark(), LQTexture((int)width, (int)height), m_VBO(0), m_VAO(0), m_FBO(0),
  m_shader(new LQShader("shaderVertex.txt", "shaderFragment.txt")),
  m_x(x), m_y(y), m_width(width), m_height(height),
  m_shapeMap(nullptr), m_clearColor()
{
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

    //framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

LQSurface::LQSurface(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLint color)
: LQSurface(x, y, width, height)
{
    m_clearColor = LQColor(color);
    clear();
}

//accesseurs
GLuint LQSurface::getVAO() const
{
    return m_VAO;
}

LQShader* LQSurface::getShader()
{
    return m_shader;
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

//méthodes
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

// void LQSurface::blit(LQTexture const& texture, GLfloat x, GLfloat y)
// {
//     glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
//     model = glm::scale(model, glm::vec3(texture.m_width, texture.m_height, 1.0f));

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

void LQSurface::blit(LQSurface const& surface) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(surface.m_x, surface.m_y, 0.0f));
    model = glm::scale(model, glm::vec3(surface.m_width, surface.m_height, 1.0f));

    m_shader->use();
    m_shader->set("model", model);
    m_shader->set("projection", glm::ortho(0.0f, m_width, m_height, 0.0f, -1.0f, 0.0f));
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