#include <litequarks/LQDocument.hpp>
#include <litequarks/LQViewport.hpp>

LQDocument::LQDocument(GLint color)
: LQViewable(0.0f, 0.0f)
{
    setClearColor(color);
}

void LQDocument::resizeHeightCallback() {
    LQViewable::resizeHeightCallback();
    if (parent()) {
        static_cast<LQViewport*>(parent())->recalc();
    }
}
