#include <litequarks/LQDeclaration.hpp>
#include <litequarks/LQTextArea.hpp>
#include <litequarks/LQAppController.hpp>
#include <litequarks/LQTreeCreator.hpp>

#include <algorithm>  // std::max

using namespace LQUnit;

LQTextArea::LQTextArea(LQNumber&& _x, LQNumber&& _y,
                       LQNumber&& _width, LQNumber&& _height,
                    GLint color, const std::string& placeholder)
// : LQText(" ", std::move(x), std::move(y), 1_em, color), m_placeholder(placeholder)
: LQViewable(std::move(_x), std::move(_y),
             std::move(_width), std::move(_height), color),
  m_placeholder(nullptr), m_text(nullptr)
{
    m_width.linkQuark<LQSurface>(*this);
    m_height.linkQuark<LQSurface>(*this);

    if (!placeholder.empty()) {
        m_placeholder = new LQText(
            placeholder, 10_px, height() / 2.0f + 6_px, 1_em, 0x000000);
        appendChild(m_placeholder);
    }
    // appendChild(m_placeholder);
    // setClearColor(0xff0000);
    // LQTexture old = resize(100_px, 25_px);
    // linkFramebuffer();
    // clear();
    // deleteTexture(old);
    // lqOn<LQHoverEvent>(this, );
    // lqOn<LQHoverLeaveEvent>(this, );
    lqOn<LQFocusGainEvent>(this, onFocusGain);
    lqOn<LQFocusLoseEvent>(this, onFocusLose);
    lqOn<LQCharEvent>(this, onChar);
}

LQTextArea::~LQTextArea() {
    delete m_placeholder;
    delete m_text;
}

void LQTextArea::onFocusGain() { }
void LQTextArea::onFocusLose() { }

void LQTextArea::onChar(LQCharEvent& event) {
    removeFirstChild();  // remove m_text or m_placeholder
    if (m_text) {  // user has already typed something, m_text is m_firstChild
        auto* old = m_text;
        m_text = new LQText(LQText::s_font.renderText(
            old->m_text + event.codepoint, 0x000000,
            10_px, height() / 2.0f + 6_px, 1_em));
        delete old;
    }
    else {  // no previous user input, m_placeholder is m_firstChild
        m_text = new LQText(LQText::s_font.renderText(
            event.codepoint, 0x000000,
            10_px, height() / 2.0f + 6_px, 1_em));
    }
    appendChild(m_text);
    // std::cout << "onchar:" <<std::endl;
    // auto ch = s_font.renderText(event.codepoint, 0x000000, widthF(),
    //                             m_baseline, 1_em);
    // std::cout << "ch "<<&ch <<std::endl;
    // m_text += event.codepoint;
    // width() = widthF() + ch.widthF();
    
    // float descender = heightF() - m_baseline;
    // float newDescender = ch.heightF() - ch.m_baseline;
    // float newHeight = std::max(m_baseline, ch.m_baseline) +
    //                   std::max(descender, newDescender);
    // if (newHeight > heightF()) {
    //     height() = newHeight;
    // }

    // std::cout << "resize" <<std::endl;
    // LQTexture old = resize(widthF(), heightF());
    // std::cout << "end resize" <<std::endl;
    // linkFramebuffer();
    // setClearColor(0x00ff00);
    // // clear();
    // // blit(old, 0.0f, std::max(0, ch.m_baseline - m_baseline), m_VAO);
    // blit(old, 0.0f, 0.0f, m_VAO);
    // blit(ch);
    // setClearColor(0x0000ff);
    // deleteTexture(old);

    // m_baseline = std::max(m_baseline, ch.m_baseline);
}
