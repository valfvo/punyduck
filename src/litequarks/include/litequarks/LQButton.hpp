#pragma once

#include <string>
#include <functional>  // std::function

#include "LQEvent.hpp"
#include "LQViewable.hpp"

class LQButton : public LQViewable {
public:
    LQButton(LQNumber&& x, LQNumber&& y, LQImageData imageData,
             const std::function<void(void)>& callback);
            //  LQViewable* target=nullptr,
            //  void (LQViewable::*callback)(LQClickEvent&)=nullptr);

    LQButton(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
             GLint color, const std::string& iconPath="",
             const std::function<void(void)>& callback = {});
            //  const std::string& iconPath="", LQViewable* target=nullptr,
            //  void (LQViewable::*callback)(LQClickEvent&)=nullptr);

    LQButton(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
             GLint color, const std::string& text, GLint textColor,
             const std::function<void(void)>& callback = {});

    void onclick(LQClickEvent& event);

    void draw() override;

private:
    LQViewable* m_target;
    std::function<void(void)> m_callback;
    // void (LQViewable::*m_callback)(LQClickEvent&);
};
