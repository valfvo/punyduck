#pragma once

#include "LQDeclaration.hpp"

class LQuark {
public:
    // LQWhiteCharge
    LQuark& parent();
    LQuark& firstChild();
    LQuark& lastChild();
    LQuark& nthChild(LQindex nth);
    LQuark& previousSibling();
    LQuark& nextSibling();
    LQuark& nthSibling(LQindex nth);

    // LQBlackCharge
    LQuark& swap(LQuark const& quark);
    LQuark& swapNextSibling();
    LQuark& swapPrevSibling();
    LQuark& swapFirstChild();
    LQuark& swapLastChild();

    // LQBlueCharge
    LQuark& appendChild(LQuark& child);
    LQuark& insertChild(LQindex index, LQuark& child);

    // LQGreenCharge
    LQuark& appendChild(LQuark& child); // O(1)
    LQuark& insertChild(LQindex index, LQuark& child); // O(n)
    LQuark& insertChildBefore(LQuark newChild, LQuark& child); // O()
    LQuark& insertChildAfter(LQuark newChild, LQuark& child); // O()

    // LQRedCharge
    LQuark& remove(); // O()
    LQuark& removeChild(LQuark& child); // O()
    LQuark& removeChild(LQindex index); // O()
    LQuark& removeFirstChild(); // O()
    LQuark& removeLastChild(); // O()
    LQuark& removeChildren(); // O(n)

protected:
    LQuark* m_parent;
    LQuark* m_prevSibling;
    LQuark* m_nextSibling;
    LQuark* m_firstChild;
    LQuark* m_lastChild;
    std::vector<LQuark*> m_children;
};
