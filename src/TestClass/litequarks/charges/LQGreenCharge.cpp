#include "../LQuark.hpp"

template<class TQuark, class ...TArgs>
LQuark& LQuark::append(TArgs ...args) {
    appendChild(new TQuark(args...));
}

LQuark& LQuark::appendChild(LQuark* child, LQenum returnEnum) {
    child->m_parent = this;
    if (m_lastChild) {
        m_lastChild->m_nextSibling = child;
        child->m_prevSibling = m_lastChild;
        m_lastChild = child;
    }
    else {
        m_firstChild = child;
        m_lastChild = child;
    }
    ++m_childrenCount;

    switch (returnEnum) {
        case LQ_RETURN_PARENT:
            return *m_parent;
        case LQ_RETURN_CHILD:
            return *child;
        default:
            return *this;
    }
}

LQuark& LQuark::appendChild(LQuark& child, LQenum returnEnum) {
    return appendChild(&child, returnEnum);
}

LQuark& LQuark::insertChild(LQindex index, LQuark& child) {
    LQuark* nth = nthChild(index+1);
    if (nth == nullptr) {
        return appendChild(child);
    }
    else {
        return insertChildBefore(child, *nth);
    }
}

LQuark& LQuark::insertChildBefore(LQuark& newChild, LQuark& child) {
    if (child.m_parent == this) {
        newChild.m_parent = this;
        if (&child == m_firstChild) {
            m_firstChild = &newChild;
        }
        else {
            child.m_prevSibling->m_nextSibling = &newChild;
            newChild.m_prevSibling = child.m_prevSibling;
        }
        child.m_prevSibling = &newChild;
        newChild.m_nextSibling = &child;
    }
    ++m_childrenCount;
    return *this;
}

LQuark& LQuark::insertChildAfter(LQuark& newChild, LQuark& child) {
    if (child.m_parent == this) {
        if (child.m_nextSibling) {
            return insertChildBefore(newChild, *child.m_nextSibling);
        }
        else {
            return appendChild(newChild);
        }
    }
}
