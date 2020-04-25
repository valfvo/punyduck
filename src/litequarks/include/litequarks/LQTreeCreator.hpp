#pragma once

template<class TRoot, class TQuark>
class LQTreeCreator {
public:
    LQTreeCreator(TRoot* root, TQuark** parent, TQuark** prevSibling);
    ~LQTreeCreator();

    LQTreeCreator<TRoot, TQuark>&
    sub();

    LQTreeCreator<TRoot, TQuark>&
    super();

    template<class TSubQuark, class ...TArgs>
    LQTreeCreator<TRoot, TQuark>&
    add(TArgs&& ...args);

private:
    TRoot*  m_root;
    TRoot*  m_quark;
    TQuark** m_currentParent;
    TQuark** m_currentPrevSibling;
};

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>
createTree(TQuark& root);

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>
createTree(TRoot& root, TQuark*& parent);

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>
createTree(TRoot& root, TQuark*& parent, TQuark*& prevSibling);

#include "LQTreeCreator.ipp"
