#include <iostream>

#include "LQEventTarget.hpp"

class LQButton : public LQEventTarget {
public:
    LQButton() : LQEventTarget(*this), color(5) { }

protected:
    void mouseMoveCallback(LQEvent& event) override {
        std::cout << "LQButton mouseMoveCallback " << color << std::endl;
    }

    int color;
};

int main() {
    LQuark* quark = new LQButton;
    LQEvent event(LQ_EVENT_01);
    quark->event.listener(event);
    delete quark;

    return EXIT_SUCCESS;
}