#include "LQuark.hpp"

LQuark::LQuark(uint64_t code)
: event(*this, code, LQEventLinker<LQuark>::data())
{
}

LQuark::LQuark(uint64_t code, LQEventCallback& callbacks)
: event(*this, code, callbacks)
{
}

LQuark::~LQuark() {
}
