#include "LQTreeCreator.hpp"

template<class TQuark>
LQTreeCreator<TQuark>::
LQTreeCreator(LQuark* root, TQuark** parent, TQuark** prevSibling)
: m_root(root), m_quark(root),
  m_currentParent(parent), m_currentPrevSibling(prevSibling)
{
    *m_currentParent = nullptr;
    *m_currentPrevSibling = nullptr;
}

template<class TQuark>
LQTreeCreator<TQuark>::
~LQTreeCreator()
{
    *m_currentParent = nullptr;
    *m_currentPrevSibling = nullptr;
}

template<class TQuark>
LQTreeCreator<TQuark>&
LQTreeCreator<TQuark>::
subTree()
{
    if (m_quark->lastChild()) {
        m_quark = m_quark->lastChild();
        *m_currentParent = static_cast<TQuark*>(m_quark);
        *m_currentPrevSibling = static_cast<TQuark*>(m_quark->lastChild());
    }
    return *this;
}

template<class TQuark>
LQTreeCreator<TQuark>&
LQTreeCreator<TQuark>::
superTree()
{
    if (m_quark != m_root) {
        m_quark = m_quark->parent();
        *m_currentParent = static_cast<TQuark*>(m_quark);
        *m_currentPrevSibling = static_cast<TQuark*>(m_quark->lastChild());
    }
    return *this;
}

template<class TQuark>
template<class TSubQuark, class ...Args>
LQTreeCreator<TQuark>&
LQTreeCreator<TQuark>::
append(Args ...args) {
    m_quark->appendChild(new TSubQuark(args...));
    *m_currentPrevSibling = static_cast<TQuark*>(m_quark->lastChild());
    return *this;
}

template<class TQuark>
LQTreeCreator<TQuark>
createTree(LQuark& root)
{
    return LQTreeCreator<TQuark>(&root, nullptr, nullptr);
}

template<class TQuark>
LQTreeCreator<TQuark>
createTree(LQuark& root, TQuark*& parent)
{
    return LQTreeCreator<TQuark>(&root, &parent, nullptr);
}

template<class TQuark>
LQTreeCreator<TQuark>
createTree(LQuark& root, TQuark*& parent, TQuark*& prevSibling)
{
    return LQTreeCreator<TQuark>(&root, &parent, &prevSibling);
}
