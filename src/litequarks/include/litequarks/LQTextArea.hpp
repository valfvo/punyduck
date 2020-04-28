#pragma once

#include <string>
#include "LQText.hpp"
#include "LQEvent.hpp"

class LQTextArea : public LQViewable {
public:
    LQTextArea(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
               GLint color, const std::string& placeholder="");
    ~LQTextArea() override;

    void onFocusGain();
    void onFocusLose();
    void onKey(LQKeyEvent& event);
    void onChar(LQCharEvent& event);

protected:
    LQText* m_placeholder;
    LQText* m_text;
};
