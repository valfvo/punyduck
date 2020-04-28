#include <litequarks/LQDeclaration.hpp>
#include <litequarks/LQTextArea.hpp>
#include <litequarks/LQAppController.hpp>
#include <litequarks/LQTreeCreator.hpp>

#include <algorithm>  // std::max

using namespace LQUnit;

LQTextArea::LQTextArea(LQNumber&& _x, LQNumber&& _y,
                       LQNumber&& _width, LQNumber&& _height,
                    GLint color, const std::string& placeholder)
: LQViewable(std::move(_x), std::move(_y),
             std::move(_width), std::move(_height), color),
  m_placeholder(nullptr), m_text(nullptr)
{
    // m_width.linkQuark<LQSurface>(*this);
    // m_height.linkQuark<LQSurface>(*this);

    if (!placeholder.empty()) {
        m_placeholder = new LQText(
            placeholder, 10_px, height() / 2.0f + 6_px, 1_em, 0x808080);
        appendChild(m_placeholder);
    }
    lqOn<LQFocusGainEvent>(this, onFocusGain);
    lqOn<LQFocusLoseEvent>(this, onFocusLose);
    lqOn<LQKeyEvent>(this, onKey);
    lqOn<LQCharEvent>(this, onChar);
}

LQTextArea::~LQTextArea() {
    delete m_placeholder;
    delete m_text;
}

void LQTextArea::onFocusGain() { }
void LQTextArea::onFocusLose() { }

void LQTextArea::onKey(LQKeyEvent& event) {
    if (event.key == GLFW_KEY_BACKSPACE &&
       (event.action == GLFW_PRESS || event.action == GLFW_REPEAT))
    {
        if (m_text) {
            removeFirstChild();
            auto& input = m_text->m_text;
            input.pop_back();
            auto* old = m_text;
            if (!input.empty()) {
                m_text = new LQText(LQText::s_font.renderText(
                    input, 0x000000, 10_px, height() / 2.0f + 6_px, 1_em));
                appendChild(m_text);
            }
            else {
                m_text = nullptr;
                appendChild(m_placeholder);
            }
            delete old;
        }
    }

    if (event.key == GLFW_KEY_ENTER && event.action == GLFW_PRESS) {
        if (m_text) {
            // target, callback(const std::string&)
        }
    }
}

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
        LQAppController::removeFocus(m_placeholder);
        m_text = new LQText(LQText::s_font.renderText(
            event.codepoint, 0x000000,
            10_px, height() / 2.0f + 6_px, 1_em));
    }
    appendChild(m_text);
}
