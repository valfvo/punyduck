#include <litequarks/litequarks.hpp>
#include "Components.hpp"

using namespace LQUnit;

DropProjectView::DropProjectView(
    LQNumber&& x, LQNumber&& y, LQNumber&& w, LQNumber&& h, GLint color)
: LQViewport(std::move(x), std::move(y), std::move(w), std::move(h), color)
{
    appendChild(new LQDocument(color));
    auto* document = static_cast<LQViewable*>(firstChild());

    document->appendChild(new LQViewable(0_px, 0_px, width(), 1_px));

    auto* drop = new LQDrop(
        0.20f * width(), 100_px, 0.60f * width(), 350_px, 0xE9E9E9);
    
    auto* text2 = new LQText("votre projet", 0.0f, drop->height() / 2, 1_em, 0x808080);
    auto* text = new LQText("Glissez et déposez", 0.0f, text2->top()-0.5_em, 1_em, 0x808080);
    auto* text3 = new LQText("ici", 0.0f, text2->bottom()+1.2_em, 1_em, 0x808080);
    text->x() = drop->width() / 2 - text->widthF() / 2;
    text2->x() = drop->width() / 2 - text2->widthF() / 2;
    text3->x() = drop->width() / 2 - text3->widthF() / 2;

    drop->appendChild(text);
    drop->appendChild(text2);
    drop->appendChild(text3);

    auto* nom = new LQTextArea(
        drop->left(), drop->bottom()+15_px, 0.60f * drop->width(),
        50_px, 0xE9E9E9, "Nom");
    auto* tag = new LQTextArea(
        nom->right() + 15_px, nom->top(), drop->width() - nom->width() - 15_px,
        50_px, 0xE9E9E9, "Tag");
    
    auto* envoyer = new LQButton(
        drop->left() + 0.30f * drop->width(), nom->bottom() + 35_px,
        0.40f * drop->width(), 50_px, 0x2f3136,"Envoyer", 0xffffff,
        [drop, nom, tag]() {
            std::string path = drop->getPath();
            std::string strNom = nom->getContent();
            std::string strTag = tag->getContent();
            auto* error2 =
                static_cast<LQViewable*>(tag->nextSibling()->nextSibling());
            auto* error = static_cast<LQViewable*>(error2->nextSibling());
            auto* valid = static_cast<LQViewable*>(error->nextSibling());
            error->hide();
            valid->hide();

            if (!path.empty() && !strNom.empty() && !strTag.empty()) {
                error2->hide();
                LQAppController::pushEvent(new upProjectEvent(
                    path, strNom, strTag, "", ""));
            }
            else {
                error2->unhide();
            }
            
        });

    auto* error2 = new LQText(
        "Veuillez remplir tous les champs", envoyer->right() + 15_px,
         envoyer->top() + envoyer->height() / 2 + 0.4_em, 1_em, 0xc93838);
    auto* error = new LQText(
        "Échec, le nom est déjà pris", envoyer->right() + 15_px,
         envoyer->top() + envoyer->height() / 2 + 0.4_em, 1_em, 0xc93838);
    auto* valid = new LQText(
        "Projet bien envoyé", envoyer->right() + 15_px,
         envoyer->top() + envoyer->height() / 2 + 0.4_em, 1_em, 0x356b34);

    error2->hide();
    error->hide();
    valid->hide();

    document->appendChild(drop);
    document->appendChild(nom);
    document->appendChild(tag);
    document->appendChild(envoyer);
    document->appendChild(error2);
    document->appendChild(error);
    document->appendChild(valid);

    static_cast<LQViewable*>(document->firstChild())->hide();
    recalc();
};
