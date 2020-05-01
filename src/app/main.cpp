#include <vector>
#include <typeindex>
#include <queue>
#include <functional>
#include <unordered_map>
#include <litequarks/litequarks.hpp>

#include <model.hpp>
#include "Components.hpp"

using namespace LQUnit;

int SCREEN_WIDTH  = 1280;
int SCREEN_HEIGHT = 720;

// LQColor BLACK (0x000000);
// LQColor WHITE (0xffffff);
// LQColor RED   (0xff0000);
// LQColor GREEN (0x00ff00);
// LQColor BLUE  (0x0000ff);

#define FVO_GREEN 0x1DBF73
// #define FVO_GREEN 0x25a65c
// #define FVO_GREY  0x283234
// #define FVO_GREY  0x282c34
#define FVO_GREY  0x36393F
// #define FVO_GREY2 0x283634
// #define FVO_GREY 0x282c34
// #define FVO_TEXT  0xacb2bf
// #define FVO_TEXT  0xe1e5ed
#define FVO_TEXT  0xdcddde

#define BACKGROUND_PRIMARY   0x36393f
#define BACKGROUND_SECONDARY 0x2f3136
#define BACKGROUND_TERNARY   0x202225

class NavBar : public LQViewable {
public:
    NavBar(LQNumber&& x, LQNumber&& y, LQNumber&& w, LQNumber&& h,
           LQViewable* projectView=nullptr, LQViewable* dropView=nullptr)
    : LQViewable(std::move(x), std::move(y), std::move(w), std::move(h), 0x2f3136),
      m_projectView(projectView), m_dropView(dropView)
    {
        auto* projectsButton =
            new LQButton(10_px, 0_px, 100_px, height(), 0x2f3136, "Projets", 0xffffff,
                [this]() {
                    parent()->removeLastChild();
                    static_cast<LQViewable*>(parent())->appendChild(m_projectView);
                    LQAppController::resetMousePosition();
                });
        projectsButton->width() = 
            static_cast<LQViewable*>(projectsButton->firstChild())->widthF();

        auto* dropButton =
            new LQButton(projectsButton->right()+10_px, 0_px, 100_px, height(), 0x2f3136,
                        "Dépôt", 0xffffff,
                [this]() {
                    parent()->removeLastChild();
                    static_cast<LQViewable*>(parent())->appendChild(m_dropView);
                    LQAppController::resetMousePosition();
                });
        dropButton->width() = 
            static_cast<LQViewable*>(dropButton->firstChild())->widthF();

        appendChild(projectsButton);
        appendChild(dropButton);
    }

private:
    LQViewable* m_projectView;
    LQViewable* m_dropView;
};

int main() {
    // TODO lqInit();
    glfwInit();
    LQAppController::init();
    LQAppModel::init();
    LQFont::init();
    LQText::init();
    initModelsConstructors();

    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    window.setClearColor(0xffffff);

    auto* projectView = 
        new ProjectView(0_px, 1_wu, window.width(), window.height() - 1_wu, 0xD9D9D9);
    auto* dropView =
        new DropProjectView(0_px, 1_wu, window.width(), window.height());

    auto* nav = new NavBar(0.0f, 0.0f, window.width(), 1_wu, projectView, dropView);
    auto* signInView =
        new SignInView(0_px, 0_px, window.width(), window.height());
    signInView->navbar = nav;
    signInView->viewport = projectView;

    window.appendChild(signInView);

    // LQAppModel::dataQuery("projectSELECT idProjet, nom, tag, pDescr, pPathImage, login FROM Projet, UserInfo WHERE pIdLog = idLog;");
    LQAppModel::dataQuery("projectSELECT idProjet, nom, tag, login FROM Projet,"
                          "UserInfo WHERE idLog = pIdLog;");

    while (window.alive()) {
        window.drawChildren();
        window.update();
    }

    LQAppController::finalize();
    glfwTerminate();
    // TODO lqFinalize(); & clear child

    return EXIT_SUCCESS;
}
