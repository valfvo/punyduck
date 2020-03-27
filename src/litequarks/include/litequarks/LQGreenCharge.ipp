#include "LQuark.hpp"

template<class TQuark, class ...Args>
inline LQuark& LQuark::append(Args ...args) {
    return appendChild(new TQuark(args...));
}
