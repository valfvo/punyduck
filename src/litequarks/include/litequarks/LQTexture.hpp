#pragma once

#include <string>
#include <glad/glad.h>

#include "LQDeclaration.hpp"

class LQTexture {
public:
    LQTexture();

    LQTexture(LQsize width, LQsize height,
              GLubyte* data=nullptr, GLenum format=GL_RGBA,
              GLint mipmapLevel=0, GLint internalFormat=GL_RGBA,
              GLenum wrapS=GL_REPEAT, GLenum wrapT=GL_REPEAT,
              GLenum minFilter=GL_LINEAR, GLenum magFilter=GL_LINEAR);

    // LQTexture(LQTexture const& texture);
    LQTexture(std::string const& path);
    LQTexture(LQTexture&& other);
    // void save(std::string const& path) const;

    //acceseurs
    GLuint getId() const;
    GLint getWidth() const;
    GLint getHeight() const;

protected:
    GLuint genTexture();
    LQTexture resize(GLuint texWidth, GLuint texHeight);

    GLuint m_id;
    GLuint m_texWidth;
    GLuint m_texHeight;
    GLint  m_format;
    GLenum m_wrapS;
    GLenum m_wrapT;
    GLenum m_minFilter;
    GLenum m_magFilter;

    friend class LQSurface;
};