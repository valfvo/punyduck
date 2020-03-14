#pragma once

#include "LQDeclaration.hpp"

class LQuark {
public:
    LQuark();
    // accessors
    LQuark* parent() const;
    LQuark* firstChild() const;
    LQuark* lastChild() const;
    LQuark* nthChild(LQindex nth) const;
    LQuark* prevSibling() const;
    LQuark* nextSibling() const;
    LQuark const* nthSibling(LQindex nth) const;
    LQsize  childrenCount() const;

    // LQGreenCharge
    template<class TQuark, class ...Args>
    LQuark& append(Args ...args);
    LQuark& appendChild(LQuark* child, LQenum returnType=LQ_RETURN_THIS);
    LQuark& appendChild(LQuark& child, LQenum returnType=LQ_RETURN_THIS);
    LQuark& insertChild(LQindex index, LQuark& child); // O(n)
    LQuark& insertChildBefore(LQuark& newChild, LQuark& child); // O()
    LQuark& insertChildAfter(LQuark& newChild, LQuark& child); // O()

    // LQRedCharge
    LQuark& detach(); // O()
    LQuark& removeChild(LQuark& child); // O()
    LQuark& removeChild(LQindex index); // O()
    LQuark& removeFirstChild(); // O()
    LQuark& removeLastChild(); // O()
    LQuark& removeChildren(LQuark& newParent); // O(n)

    // LQYellowCharge
    LQuark& swapWith(LQuark& quark); // O()
    LQuark& swapChildren(LQuark& first, LQuark& second); // O()
    LQuark& swapChildren(LQindex first, LQindex second); // O()

protected:
    LQuark* m_parent;
    LQuark* m_prevSibling;
    LQuark* m_nextSibling;
    LQuark* m_firstChild;
    LQuark* m_lastChild;
    LQsize  m_childrenCount;

    friend class LQSurface;
};

#include "LQGreenCharge.ipp"
