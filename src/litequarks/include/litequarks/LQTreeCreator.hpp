#pragma once

#include "LQuark.hpp"

template<class TQuark>
class LQTreeCreator {
public:
    LQTreeCreator(LQuark* root, TQuark** parent, TQuark** prevSibling);
    ~LQTreeCreator();

    LQTreeCreator<TQuark>&
    subTree();

    LQTreeCreator<TQuark>&
    superTree();

    template<class TSubQuark, class ...Args>
    LQTreeCreator<TQuark>&
    append(Args ...args);

private:
    LQuark*  m_root;
    LQuark*  m_quark;
    TQuark** m_currentParent;
    TQuark** m_currentPrevSibling;
};

template<class TQuark>
LQTreeCreator<TQuark>
createTree(LQuark& root);

template<class TQuark>
LQTreeCreator<TQuark>
createTree(LQuark& root, TQuark*& parent);

template<class TQuark>
LQTreeCreator<TQuark>
createTree(LQuark& root, TQuark*& parent, TQuark*& prevSibling);

#include "LQTreeCreator.ipp"
