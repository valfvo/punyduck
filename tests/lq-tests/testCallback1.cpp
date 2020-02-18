#include <cstdint>

#include <iostream>

class LQuark;
class LQEvent;
class LQButton;

class LQuark {
public:
    LQuark(unsigned int code) : eventCodeListener(code) { }
    void eventListener(LQEvent& event) {
        if (eventCodeListener & event.code) {
            // (*eventCallbacks[event.code])(event);
            if (propagateCaughtEvent) {
                // propagate below
            }
        }
        if (propagateEvent) {
            // propagate below
        }
    }

    template<typename T, int eventCode>
    void eventCallback(LQEvent& event);

private:
    uint64_t eventCodeListener;
    bool propagateEvent;
    bool propagateCaughtEvent;
    static void (*eventCallbacks[64])(LQEvent& event);
};

class LQEvent {
public:
    LQEvent(int code) : code(code) { }
    uint64_t const code;
};

class LQButton : public LQuark {
public:
    LQButton(unsigned int code) : LQuark(code) { }
};

template<>
void LQuark::eventCallback<LQButton, 1>(LQEvent& event) {
    std::cout << "LQButton eventCallback 1" << std::endl;
};

int main() {
    LQuark* quark = new LQButton(1);
    LQEvent event(1);
    quark->eventListener(event);
    delete quark;

    return EXIT_SUCCESS;
}