#include "LQuark.hpp"

template<class TQuark, class ...Args>
LQuark& LQuark::append(Args ...args) {
    appendChild(new TQuark(args...));
}
