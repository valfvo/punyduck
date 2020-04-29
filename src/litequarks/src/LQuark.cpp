#include <litequarks/LQuark.hpp>

LQuark::LQuark()
: m_parent(nullptr), m_prevSibling(nullptr), m_nextSibling(nullptr),
  m_firstChild(nullptr), m_lastChild(nullptr), m_childrenCount(0)
{
}

LQuark::~LQuark() { }

LQuark* LQuark::parent() const {
    return m_parent;
}

LQuark* LQuark::firstChild() const {
    return m_firstChild;
}

LQuark* LQuark::lastChild() const {
    return m_lastChild;
}

LQuark* LQuark::nthChild(LQindex nth) const {
    LQuark* child = nullptr;
    if (nth < (m_childrenCount - nth)) {
        child = m_firstChild;
        for (LQindex i = 0; i != nth; ++i) {
            child = child->m_nextSibling;
        }
    }
    else {
        child = m_lastChild;
        for (LQindex i = m_childrenCount-1; i != nth; --i) {
            child = child->m_prevSibling;
        }
    }
    return child;
}

LQuark* LQuark::prevSibling() const {
    return m_prevSibling;
}

LQuark* LQuark::nextSibling() const {
    return m_nextSibling;
}

LQuark const* LQuark::nthSibling(LQindex nth) const {
    LQuark const* sibling = this;
    if (nth < 0) {
        for (LQindex i = nth; sibling && i >= 0; --i) {
            sibling = sibling->m_prevSibling;
        }
    }
    else {
        for (LQindex i = nth; sibling && i >= 0; --i) {
            sibling = sibling->m_nextSibling;
        }
    }
    return sibling;
}

LQsize LQuark::childrenCount() const {
    return m_childrenCount;
}

void LQuark::setNextSibling(LQuark* nextSibling) {
    m_nextSibling = nextSibling;
}
