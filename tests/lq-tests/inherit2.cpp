#include <iostream>
#include <memory>

/*
<app>
    <surface width='100' height='200'>
        <button cx='$(../../1/square{ $x + $y })' cy='..$cy' width='10' height='10' color='blue'>
            <!-- . -->
        </button>
        <!-- .. -->
    </surface>
    <surface width='200' height='400' x='150'>
        <square width='20' x='$y' y='50'>
    </surface>
</app>
*/

class BlueCharge;
class DisabledBlueCharge;

class LiteCharge {
    void callLite() { };
};

class BlackCharge : LiteCharge {
public:
    void callBlack() { };
};

class BlueCharge {
public:
    BlueCharge() : m_origin(this) { }
    inline void callBlue() const { m_origin->_callBlue(); }
    void disable();
    void enable() { m_origin = this; }

private:
    virtual void _callBlue() const { std::cout << "blue called" << std::endl; }

    BlueCharge const* m_origin;
};

class DisabledBlueCharge : public BlueCharge {
    void _callBlue() const {
        std::clog << "blue charge is disabled" << std::endl;
        exit(EXIT_FAILURE);
    }
};

DisabledBlueCharge const DBC;

void BlueCharge::disable() {
    m_origin = &DBC;
}

class GreenCharge {
public:
    void callGreen() { };
};

class RedCharge {
public:
    void callRed() { };
};

class Quark {
public:
    BlueCharge blue;
    // std::shared_ptr<GreenCharge> green;
    // std::shared_ptr<RedCharge> red;
    Quark* origin;
};

int main() {
    Quark A;
    A.blue.callBlue();  // cout << blue called
    A.blue.disable();
    A.blue.callBlue();  // callBlue est une autre fonction

    // A.blue->callBlue();
    // A.red->removeBelow();
    // A.green->addLast(last);

    return EXIT_SUCCESS;
}