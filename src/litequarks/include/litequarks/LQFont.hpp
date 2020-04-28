#pragma once

#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "LQDeclaration.hpp"
#include "LQNumber.hpp"
#include "LQTexture.hpp"

class LQFont;

#include "LQText.hpp"

struct LQGlyph {
    LQTexture bitmap;
    GLint     left;
    GLint     top;
    GLint     advanceX;
};

class LQFont {
public:
    LQFont();
    LQFont(const std::string& fontname,
           LQNumber&& size=LQUnit::operator""_em(1ULL));
    ~LQFont();

    void change(const std::string& fontname,
                LQNumber&& size=LQUnit::operator""_em(1ULL));

    const LQGlyph& renderChar(char32_t character);
    LQText renderText(const std::u32string& text, GLuint color=0x000000,
                      LQNumber&& x=0.0f, LQNumber&& y=0.0f,
                      LQNumber&& size=LQUnit::operator""_em(1ULL));

    static void init();

private:
    FT_Face m_face;
    std::unordered_map<char32_t, LQGlyph> m_glyphs;

    static FT_Library s_library;
};
