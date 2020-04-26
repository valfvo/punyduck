#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include <litequarks/LQTexture.hpp>

//constructeurs
LQTexture::LQTexture()
: m_id(0), m_texWidth(0), m_texHeight(0), m_format(GL_RGBA),
  m_wrapS(GL_REPEAT), m_wrapT(GL_REPEAT),
  m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR) { }

LQTexture::LQTexture(
    LQsize width, LQsize height,
    GLubyte* data, GLenum format, GLint mipmapLevel, GLint internalFormat,
    GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter)
: m_id(0), m_texWidth(width), m_texHeight(height), m_format(internalFormat),
  m_wrapS(wrapS), m_wrapT(wrapT),
  m_minFilter(minFilter), m_magFilter(magFilter)
{
    if (width >= 1 && height >= 1) {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_texWidth, m_texHeight, 0,
                    format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
// LQTexture::LQTexture(LQTexture const& texture)
// : m_id(0), m_texWidth(texture.m_texWidth), m_texHeight(texture.m_texHeight),
//   m_format(texture.m_format),
//   m_wrapS(texture.m_wrapS), m_wrapT(texture.m_wrapT),
//   m_minFilter(texture.m_minFilter), m_magFilter(texture.m_magFilter)
// {
//     if (this != &texture)
//     {
        
//     }
// }
LQTexture::LQTexture(std::string const& path, int width, int height)
: m_id(0), m_texWidth(0), m_texHeight(0), m_format(GL_RGBA),
  m_wrapS(GL_REPEAT), m_wrapT(GL_REPEAT),
  m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

    int img_width, img_height, img_format;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* data = stbi_load(path.c_str(), &img_width, &img_height, &img_format, 0);

    if (data && (img_format == 3 || img_format == 4)) {
        if (width >= 1 && height >= 1) {
            GLubyte* resized = new GLubyte[width*height*img_format];
            stbir_resize_uint8(data, img_width, img_height, 0,
                               resized, width, height, 0, img_format);
            stbi_image_free(data);
            data = resized;
            img_width = width;
            img_height = height;
        }
        m_texWidth = img_width;
        m_texHeight = img_height;
        m_format = img_format == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_texWidth, m_texHeight, 0,
                     m_format, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

LQTexture::LQTexture(LQTexture&& other)
: m_id(other.m_id), m_texWidth(other.m_texWidth), m_texHeight(other.m_texHeight),
  m_format(other.m_format), m_wrapS(other.m_wrapS), m_wrapT(other.m_wrapT),
  m_minFilter(other.m_minFilter), m_magFilter(other.m_magFilter)
{
    other.m_id = 0;
}

// void LQTexture::save(const std::string& path) const
// {
// }

//acceseurs
GLuint LQTexture::getId() const {
    return m_id;
}

GLint LQTexture::getWidth() const {
    return m_texWidth;
}

GLint LQTexture::getHeight() const {
    return m_texHeight;
}

GLuint LQTexture::genTexture() {
    GLuint id = 0;
    if (m_texWidth >= 1 && m_texHeight >= 1) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_texWidth, m_texHeight, 0,
                    m_format, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    return id;
}

LQTexture LQTexture::resize(GLuint texWidth, GLuint texHeight) {

    LQTexture old;
    old.m_id = m_id;
    old.m_texWidth = m_texWidth;
    old.m_texHeight = m_texHeight;

    m_texWidth = texWidth;
    m_texHeight = texHeight;
    m_id = genTexture();

    return old;
}
