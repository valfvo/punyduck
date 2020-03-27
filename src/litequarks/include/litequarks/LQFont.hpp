#pragma once

#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "LQTexture.hpp"
#include "LQSurface.hpp"

struct LQGlyph {
    LQTexture bitmap;
    GLint     left;
    GLint     top;
    GLint     advanceX;
};

class LQFont {
public:
    LQFont(std::string fontname);
    ~LQFont();

    const LQGlyph& renderChar(char32_t character);
    LQSurface renderText(std::u32string text, GLuint color=0x000000);

private:
    void initLibrary();

    FT_Face m_face;
    std::unordered_map<char32_t, LQGlyph> m_glyphs;

    static FT_Library s_library;
};
