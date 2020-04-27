#include <utility>  // std::forward
#include "LQTreeCreator.hpp"

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>::
LQTreeCreator(TRoot* root, TQuark** parent, TQuark** prevSibling)
: m_root(root), m_quark(root),
  m_currentParent(parent), m_currentPrevSibling(prevSibling)
{
    *m_currentParent = static_cast<TQuark*>(root);
    *m_currentPrevSibling = nullptr;
}

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>::
~LQTreeCreator()
{
    *m_currentParent = nullptr;
    *m_currentPrevSibling = nullptr;
}

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>&
LQTreeCreator<TRoot, TQuark>::
sub()
{
    if (m_quark->lastChild()) {
        m_quark = static_cast<TRoot*>(m_quark->lastChild());
        *m_currentParent = static_cast<TQuark*>(m_quark);
        *m_currentPrevSibling = static_cast<TQuark*>(m_quark->lastChild());
    }
    return *this;
}

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>&
LQTreeCreator<TRoot, TQuark>::
super()
{
    if (m_quark != m_root) {
        m_quark = static_cast<TRoot*>(m_quark->parent());
        *m_currentParent = static_cast<TQuark*>(m_quark);
        *m_currentPrevSibling = static_cast<TQuark*>(m_quark->lastChild());
    }
    return *this;
}

template<class TRoot, class TQuark>
template<class TSubQuark, class ...TArgs>
LQTreeCreator<TRoot, TQuark>&
LQTreeCreator<TRoot, TQuark>::
add(TArgs&& ...args) {
    m_quark->appendChild(new TSubQuark(std::forward<TArgs>(args)...));
    *m_currentPrevSibling = static_cast<TQuark*>(m_quark->lastChild());
    return *this;
}

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>
createTree(TRoot& root)
{
    return LQTreeCreator<TRoot, TQuark>(&root, nullptr, nullptr);
}

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>
createTree(TRoot& root, TQuark*& parent)
{
    return LQTreeCreator<TRoot, TQuark>(&root, &parent, nullptr);
}

template<class TRoot, class TQuark>
LQTreeCreator<TRoot, TQuark>
createTree(TRoot& root, TQuark*& parent, TQuark*& prevSibling)
{
    return LQTreeCreator<TRoot, TQuark>(&root, &parent, &prevSibling);
}
