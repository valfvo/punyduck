#include <iostream>
#include <vector>
#include <typeindex>
#include <queue>
#include <functional>
#include <unordered_map>
#include <litequarks/litequarks.hpp>
#include <model.hpp>

#include "Components.hpp"
// using namespace LQUnit;

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

class TestAffiche {
public:
    TestAffiche() {
        LQ_FOR_EACH(Project, afficheProject);
    }

    void afficheProject(Project* project) {
        std::cout << "nous allons afficher:\nnom: "
                  << project->nom << " desc: " << project->desc << std::endl;
    }
};

int main() {
    // TODO lqInit();
    glfwInit();
    LQAppController::init();
    LQAppModel::init();

    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    window.appendChild(ProjectView(0.0f, 0.0f, window.width(), window.height()));
    window.clear();
    window.drawChildren();

    TestAffiche test;

    // LQAppModel::dataQuery("projectSELECT idProject, nom, tag, pDescr, pPathImage, login FROM Projet, UserInfo WHERE pIdLog = idLog;");
    // int action;
    // std::cout << "Choissisez une action (1 login 2 register 3 upProjet 4 dlProject)" << std::endl;
    // std::cin >> action;
    // LQAppController::pushEvent(new tempActionEvent(action));

    while (window.alive()) {
        window.update();
    }

    LQAppController::finalize();
    glfwTerminate();
    // TODO lqFinalize();

    return EXIT_SUCCESS;
}
