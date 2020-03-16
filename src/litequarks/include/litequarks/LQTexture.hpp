#pragma once

#include <string>
#include <glad/glad.h>

class LQTexture {
public:
    LQTexture();
    LQTexture(GLuint width, GLuint height);
    // LQTexture(LQTexture const& texture);
    LQTexture(std::string const& path);
    // void save(std::string const& path) const;

    //acceseurs
    GLuint getId() const;
    GLuint getWidth() const;
    GLuint getHeight() const;

protected: 
    GLuint m_id;
    GLuint m_width;
    GLuint m_height;
    GLint  m_format;
    GLenum m_wrapS;
    GLenum m_wrapT;
    GLenum m_minFilter;
    GLenum m_magFilter;

    friend class LQSurface;
};