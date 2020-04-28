#include <litequarks/LQDeclaration.hpp>
#include <litequarks/LQText.hpp>

std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>
LQText::s_convert;

LQFont
LQText::s_font;

LQText::LQText(const std::u32string& text, LQNumber&& _x, LQNumber&& _y,
               LQNumber&& w, LQNumber&& h, int baseline)
: LQViewable(std::move(_x), std::move(_y), std::move(w), std::move(h)),
  m_text(text), m_baseline(baseline) { }

LQText::LQText(const std::string& text, LQNumber&& _x, LQNumber&& _y,
               LQNumber&& h, GLint color)
: LQText(s_font.renderText(
    s_convert.from_bytes(text), color,
    std::move(_x), std::move(_y), std::move(h))) { }

int LQText::baseline() {
    return m_baseline;
}

void LQText::init() {
    s_font.change("Montserrat-Regular");
}

void LQText::draw() { }
