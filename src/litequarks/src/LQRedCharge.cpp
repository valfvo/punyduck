#include <litequarks/LQuark.hpp>

LQuark& LQuark::detach() {
    if (m_prevSibling) {
        m_prevSibling->m_nextSibling = m_nextSibling;
    }
    else {
        m_parent->m_firstChild = m_nextSibling;
    }

    if (m_nextSibling) {
        m_nextSibling->m_prevSibling = m_prevSibling;
    }
    else {
        m_parent->m_lastChild = m_prevSibling;
    }

    m_parent = nullptr;
    m_prevSibling = nullptr;
    m_nextSibling = nullptr;

    return *this;
}

LQuark& LQuark::removeChild(LQuark& child) {
    if (child.m_parent == this) {
        return child.detach();
    }
    return *this;
}

LQuark& LQuark::removeChild(LQindex index) {
    LQuark* child = nthChild(index);
    if (child) {
        return child->detach();
    }
    return *this;
}

LQuark& LQuark::removeFirstChild() {
    if (m_firstChild) {
        m_firstChild->detach();
    }
    return *this;
}

LQuark& LQuark::removeLastChild() {
    if (m_lastChild) {
        m_lastChild->detach();
    }
    return *this;
}

LQuark& LQuark::removeChildren(LQuark& newParent) {
    if (m_firstChild) {
        for (
            LQuark* child = m_firstChild;
            child != nullptr;
            child = child->m_nextSibling
        ) {
            newParent.appendChild(child);
        }
        m_firstChild = nullptr;
        m_lastChild = nullptr;
    }
    return *this;
}
