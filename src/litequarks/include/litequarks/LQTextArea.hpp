#pragma once

#include <functional>  // std::function
#include <string>
#include "LQText.hpp"
#include "LQEvent.hpp"

class LQTextArea : public LQViewable {
public:
    LQTextArea(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
               GLint color, const std::string& placeholder="",
               const std::function<void(const std::string&)>& callback = {});
    ~LQTextArea() override;

    void setCallback(const std::function<void(const std::string&)>& callback);

    void onFocusGain();
    void onFocusLose();
    void onKey(LQKeyEvent& event);
    void onChar(LQCharEvent& event);

protected:
    LQText* m_placeholder;
    LQText* m_text;
    std::function<void(const std::string&)> m_callback;
};
