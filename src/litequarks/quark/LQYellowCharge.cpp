#include "LQuark.hpp"

LQuark& LQuark::swapWith(LQuark& quark) {
    LQuark* thisParent = m_parent;
    LQuark* thisPrevSibling = m_prevSibling;
    LQuark* thisNextSibling = m_nextSibling;

    m_parent = quark.m_parent;
    m_prevSibling = quark.m_prevSibling;
    m_nextSibling = quark.m_nextSibling;

    quark.m_parent = thisParent;
    quark.prevSibling = thisPrevSibling;
    quark.nextSibling = thisNextSibling;

    return *this;
}

LQuark& LQuark::swapChildren(LQuark& first, LQuark& second) {
    if (first.m_parent == this && second.m_parent == this) {
        first.swapWith(second);
    }
    return *this;
}

LQuark& LQuark::swapChildren(LQindex first, LQindex second) {
    LQuark* firstChild = nthChild(first);
    LQuark* secondChild = nthChild(second);

    if (firstChild && secondChild
        && firstChild->m_parent == this && secondChild->m_parent == this
    ) {
        firstChild->swapWith(*secondChild);
    }
    return *this;
}
