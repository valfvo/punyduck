#include <litequarks/LQDrop.hpp>
#include <litequarks/LQAppController.hpp>
#include <litequarks/LQText.hpp>

using namespace LQUnit;

LQDrop::LQDrop(LQNumber&& _x, LQNumber&& _y,
               LQNumber&& _width, LQNumber&& _height, GLint color)
: LQViewable(std::move(_x), std::move(_y),
             std::move(_width), std::move(_height), color)
{
    lqOn<LQDropEvent>(this, onDrop);
}

void LQDrop::onDrop(LQDropEvent& event) {
    if (firstChild()->nextSibling()->nextSibling()->nextSibling()) {
        auto* last = lastChild();
        removeLastChild();
        delete last;
    }
    LQText* message = nullptr;
    if (event.paths.size() != 1) {
        message = new LQText("Veuillez déposer un seul dossier ou fichier", 0.0f,
                         height() - 5_em, 1_em, 0xc93838);
        m_path = "";
    }
    else {
        message = new LQText(event.paths[0], 0.0f, height()-5_em, 1_em, 0x356b34);
        m_path = event.paths[0];
    }
    message->x() = width() / 2 - message->widthF() / 2;
    appendChild(message);
}

std::string LQDrop::getPath() const {
    return m_path;
}
