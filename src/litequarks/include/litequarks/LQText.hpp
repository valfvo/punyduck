#pragma once

#include <codecvt>  // std::codecvt_utf8
#include <locale>  // std::wstring_convert

#include <string>

#include "LQDeclaration.hpp"
#include "LQViewable.hpp"

class LQText;

#include "LQFont.hpp"

class LQText : public LQViewable {
public:
    LQText(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
           int baseline);
    LQText(const std::string& text, LQNumber&& x, LQNumber&& y,
           LQNumber&& height=LQUnit::operator""_em(1ULL), GLint color=0xffffff);
    LQText(LQText&& other) = default;
    LQText(const LQText& other) = delete;

    int baseline();

    void draw() override;

    static void init();

    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>
    s_convert;

    static LQFont
    s_font;

private:
    std::u32string m_text;
    int m_baseline;
};
