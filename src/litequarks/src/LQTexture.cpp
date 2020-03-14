#include <iostream>
#include <litequarks/LQTexture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//constructeurs
LQTexture::LQTexture()
: m_id(0), m_width(0), m_height(0), m_format(GL_RGBA),
  m_wrapS(GL_REPEAT), m_wrapT(GL_REPEAT),
  m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR)
{
}

LQTexture::LQTexture(GLuint width, GLuint height)
: m_id(0), m_width(width), m_height(height), m_format(GL_RGBA),
  m_wrapS(GL_REPEAT), m_wrapT(GL_REPEAT),
  m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// LQTexture::LQTexture(LQTexture const& texture)
// : m_id(0), m_width(texture.m_width), m_height(texture.m_height),
//   m_format(texture.m_format),
//   m_wrapS(texture.m_wrapS), m_wrapT(texture.m_wrapT),
//   m_minFilter(texture.m_minFilter), m_magFilter(texture.m_magFilter)
// {
//     if (this != &texture)
//     {
        
//     }
// }

LQTexture::LQTexture(std::string const& path)
: m_id(0), m_width(0), m_height(0), m_format(GL_RGBA),
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
    GLubyte* data = stbi_load(path.c_str(), &img_width, &img_height, &img_format, 0);
    if (data)
    {
        m_width = img_width;
        m_height = img_height;
        m_format = img_format;
        glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// void LQTexture::save(const std::string& path) const
// {
// }

//acceseurs
GLuint LQTexture::getId() const
{
    return m_id;
}
GLuint LQTexture::getWidth() const
{
    return m_width;
}
GLuint LQTexture::getHeight() const
{
    return m_height;
}