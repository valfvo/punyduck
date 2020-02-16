#include <iostream>

#include "LQEventTarget.hpp"

class LQButton;

// class LQButton : public LQEventListener<LQButton> {
// class LQButton : public LQuark {
class LQButton : public LQEventTarget {
public:
    LQButton(uint64_t code)
    : LQEventTarget(*this, code), color(5)
    {
    }

protected:
    void mouseMoveCallback(LQEvent& event) override {
        std::cout << "LQButton mouseMoveCallback " << color << std::endl;
    }

    int color;
};

int main() {
    // LQButton::linkEventCallbacks();
    LQuark* quark = new LQButton(1);
    // // auto button = new LQButton(1);
    LQEvent event(LQ_EVENT_00);
    quark->event.listener(event);
    delete quark;

    return EXIT_SUCCESS;
}