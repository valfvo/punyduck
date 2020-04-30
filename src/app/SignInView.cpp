#include <litequarks/litequarks.hpp>
#include "Components.hpp"

using namespace LQUnit;

SignInView::SignInView(
    LQNumber&& x, LQNumber&& y, LQNumber&& w, LQNumber&& h, GLint color)
: LQViewport(std::move(x), std::move(y), std::move(w), std::move(h), color)
{
    appendChild(new LQDocument(color));
    auto* document = static_cast<LQViewable*>(firstChild());

    document->appendChild(new LQViewable(0_px, 0_px, width(), 1_px));

    auto* icon = new IMG(width()/2.0f - 50_px, 200_px, 100_px, 100_px, color,"defaultpp.png");
    auto* login = new LQTextArea(0.32f * width(), icon->bottom() + 40_px,
                                 0.36f * width(), 50_px, 0xE9E9E9, "Nom d'utilisateur");
    auto* password = new PasswordArea(0.32f * width(), login->bottom() + 25_px,
                                      0.36f * width(), 50_px, 0xE9E9E9, "Mot de passe");

    auto* signIn = new LQButton(
        password->x() + 0.20f * password->width(), password->bottom() + 70_px,
        0.30f * password->width() - 10_px, 50_px, 0x2f3136, "Se connecter", 0xffffff,
        [login, password]() {
            std::string strLogin = login->getContent();
            std::string strPassword = password->getContent();

            auto* errorSignIn = static_cast<LQViewable*>(
                password->nextSibling()->nextSibling()->nextSibling());
            auto* errorSignUp = static_cast<LQViewable*>(errorSignIn->nextSibling());
            auto* errorField = static_cast<LQViewable*>(errorSignUp->nextSibling());
            errorSignIn->hide();
            errorSignUp->hide();

            if (!strLogin.empty() && !strPassword.empty()) {
                errorField->hide();
                LQAppController::pushEvent(new loginEvent(
                    login->getContent(), password->getContent()));
            }
            else {
                errorField->unhide();
            }
        });

    auto* signUp = new LQButton(
        password->x() + 0.50f * password->width() + 10_px, password->bottom() + 70_px,
        0.30f * password->width(), 50_px, 0x2f3136, "S'inscrire", 0xffffff,
        [login, password]() {
            std::string strLogin = login->getContent();
            std::string strPassword = password->getContent();

            auto* errorSignIn = static_cast<LQViewable*>(
                password->nextSibling()->nextSibling()->nextSibling());
            auto* errorSignUp = static_cast<LQViewable*>(errorSignIn->nextSibling());
            auto* errorField = static_cast<LQViewable*>(errorSignUp->nextSibling());
            errorSignIn->hide();
            errorSignUp->hide();

            if (!strLogin.empty() && !strPassword.empty()) {
                errorField->hide();
                LQAppController::pushEvent(new registerEvent(
                    login->getContent(), password->getContent()));
            }
            else {
                errorField->unhide();
            }
        });

    auto* errorSignIn = new LQText(
        "Identifiants incorrects", 0.0f, password->bottom() + 40_px, 1_em, 0xc93838);
    errorSignIn->x() = document->width() / 2 - errorSignIn->widthF() / 2;

    auto* errorSignUp = new LQText(
        "Ce nom est déjà pris", 0.0f, password->bottom() + 40_px, 1_em, 0xc93838);
    errorSignUp->x() = document->width() / 2 - errorSignUp->widthF() / 2;

    auto* errorField = new LQText(
        "Veuillez remplir tous les champs", 0.0f, password->bottom() + 40_px, 1_em, 0xc93838);
    errorField->x() = document->width() / 2 - errorField->widthF() / 2;

    errorSignIn->hide();
    errorSignUp->hide();
    errorField->hide();

    document->appendChild(icon);
    document->appendChild(login);
    document->appendChild(password);
    document->appendChild(signIn);
    document->appendChild(signUp);
    document->appendChild(errorSignIn);
    document->appendChild(errorSignUp);
    document->appendChild(errorField);

    static_cast<LQViewable*>(document->firstChild())->hide();
    recalc();
}

PasswordArea::PasswordArea(
    LQNumber&& _x, LQNumber&& _y, LQNumber&& _width, LQNumber&& _height,
    GLint color, const std::string& placeholder,
    const std::function<void(const std::string&)>& callback)
: LQTextArea(std::move(_x), std::move(_y), std::move(_width), std::move(_height),
            color, placeholder, callback), m_password("") { }

void PasswordArea::onKey(LQKeyEvent& event) {
    if (event.key == GLFW_KEY_BACKSPACE &&
       (event.action == GLFW_PRESS || event.action == GLFW_REPEAT))
    {
        if (!m_password.empty()) {
            m_password.pop_back();
        }
    }
    LQTextArea::onKey(event);
}

void PasswordArea::onChar(LQCharEvent& event) {
    LQCharEvent hiddenCharEvent(this, U'*');
    LQTextArea::onChar(hiddenCharEvent);
    m_password += LQText::s_convert.to_bytes(event.codepoint);
}

std::string PasswordArea::getContent() const {
    return m_password;
}
