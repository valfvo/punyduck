#include <iostream>
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
        // drop->left() + 0.30f * drop->width(), nom->bottom() + 35_px,
        // 0.40f * drop->width(), 50_px, 0x2f3136,"Envoyer", 0xffffff,
        // [drop, nom, tag]() {

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

    // LQSurface *parent, *prev;
    // createTree(window, parent, prev)
    auto* nav = new NavBar(0.0f, 0.0f, window.width(), 1_wu, projectView, dropView);
    // window.appendChild(projectView);
    auto* signInView =
        new SignInView(0_px, 0_px, window.width(), window.height());
    signInView->navbar = nav;
    signInView->viewport = projectView;

    window.appendChild(signInView);
    // window.appendChild(new SignInView(0_px, 0_px, window.width(), window.height()));

    // LQAppModel::dataQuery("projectSELECT idProjet, nom, tag, pDescr, pPathImage, login FROM Projet, UserInfo WHERE pIdLog = idLog;");
    LQAppModel::dataQuery("projectSELECT idProjet, nom, tag, login FROM Projet,"
                          "UserInfo WHERE idLog = pIdLog;");

    // int action;
    // std::cout << "Choissisez une action (1 login 2 register 3 upProjet 4 dlProject)" << std::endl;
    // std::cin >> action;
    // LQAppController::pushEvent(new tempActionEvent(action));

    // UL_Project ul(0.0f, 0.0f, 0.0f);
    // using namespace std::placeholders;
    // std::function<void(std::vector<Project*>&)> oui =
    //     std::bind(&UL_Project::searchCallback, &ul, _1);
    // // std::function<void(std::vector<Project*>&)> oui = [](std::vector<Project*>& v) { };
    // const std::string input = "oui";

    // auto lb = std::function<bool(Project*)>([](Project* project) {
    //     return project->nom.find("") != std::string::npos;
    // });

    // [parent, lb](const std::string& input) {
    //     LQAppModel::itemQuery("project",
    //         static_cast<UL_Project*>(parent->prevSibling()),
    //         UL_Project::searchCallback, lb);
    // };

    // std::function<bool(Project*)> non = [input](Project* project) {
    //     return project->nom.find(input) != std::string::npos;
    // };

    // LQAppModel::itemQuery<Project, UL_Project>(input, &ul, oui, [](Project* project) { return true; });
    // LQAppModel::itemQuery(input, &ul, oui, [](Project* project) { return true; });

    // LQAppModel::itemQuery<Project, UL_Project, UL_Project::searchCallback>(
    //     input, &ul);
    // LQAppModel::itemQuery<Project, UL_Project>(input, &ul);

    // auto lb = [&ul, non](const std::string& input) {
        // LQAppModel::itemQuery<Project, UL_Project>(input, &ul, &UL_Project::searchCallback, non);
        // LQAppModel::itemQuery<Project, UL_Project>(input, &ul, &UL_Project::searchCallback);
    // };

    // window.drawChildren();
    while (window.alive()) {
        window.drawChildren();
        window.update();
    }

    LQAppController::finalize();
    glfwTerminate();
    // TODO lqFinalize(); & clear child

    return EXIT_SUCCESS;
}
