#include <litequarks/LQuark.hpp>

LQuark& LQuark::swapWith(LQuark& quark) {
    LQuark* thisParent = m_parent;
    LQuark* thisPrevSibling = m_prevSibling;
    LQuark* thisNextSibling = m_nextSibling;

    m_parent = quark.m_parent;
    m_prevSibling = quark.m_prevSibling;
    m_nextSibling = quark.m_nextSibling;

    quark.m_parent = thisParent;
    quark.m_prevSibling = thisPrevSibling;
    quark.m_nextSibling = thisNextSibling;

    return *this;
}

LQuark& LQuark::swapChildren(LQuark& first, LQuark& second) {
    if (first.m_parent == this && second.m_parent == this) {
        first.swapWith(second);
    }
    return *this;
}

LQuark& LQuark::swapChildren(LQindex first, LQindex second) {
    LQuark* p_first = nthChild(first);
    LQuark* p_second = nthChild(second);

    if (p_first && p_second
        && p_first->m_parent == this && p_second->m_parent == this
    ) {
        p_first->swapWith(*p_second);
    }
    return *this;
}
