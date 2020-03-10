#include <iostream>

#include "LQSurface.hpp"

void printChildren(LQuark const& quark, int tabCount=0) {
    for (
        LQuark* child = quark.firstChild();
        child != nullptr;
        child = child->nextSibling()
    ) {
        std::cout << std::string(tabCount, '\t') << "quark" << std::endl;
        printChildren(*child, tabCount+1);
    }
}

int main() {
    LQuark quark;
    quark
    .appendChild(new LQuark(), LQ_RETURN_CHILD)
        .appendChild(new LQuark())
        .appendChild(new LQuark())
        .appendChild(new LQuark())
        .appendChild(new LQuark())
        .appendChild(new LQuark())
        .appendChild(new LQuark(), LQ_RETURN_PARENT)
    .appendChild(new LQuark())
    .appendChild(new LQuark());

    printChildren(quark);

    return 0;
}
