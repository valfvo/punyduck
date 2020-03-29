#include <iostream>
#include <vector>
#include <litequarks/litequarks.hpp>

using namespace LQUnit;

int SCREEN_WIDTH  = 1280;
int SCREEN_HEIGHT = 720;

LQColor BLACK (0x000000);
LQColor WHITE (0xffffff);
LQColor RED   (0xff0000);
LQColor GREEN (0x00ff00);
LQColor BLUE  (0x0000ff);

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

#define LQ_PARSE_MODEL(name)\
    static_cast<name##Model*>(lq_modelData.parse(#name))

#define LQ_CREATE_VIEW(models...)\
    lqCreateView(#models, [](LQModelData lq_modelData)

class LQModelData {
public:
    void* parse(std::string name) {
        return nullptr;
    }
};

// using LQImage = LQViewable;
using LQView = LQViewable;
using LQViewConstructor = LQView (*)(LQModelData data);
// class LQText;

struct ProjectModel {
    char* titre;
    char* desc;
    char* image;
};

struct ProfileModel {
    int   n;
    char* name;
    char* bio;
    int   age;
};

// class LQRawData {

// };

void lqCreateModel(std::string name, void* (*constructor)(LQRawData data)) {
    // LQModelData data;
    // return constructor(data);
}

LQView lqCreateView(std::string models, LQViewConstructor constructor) {
    LQModelData data;
    return constructor(data);
}

int main2() {
    LQ_CREATE_MODEL(Project) {
        return new ProjectModel{
            LQ_PARSE(char*),
            LQ_PARSE(char*),
            LQ_PARSE(char*)};
    });

    LQView Accueil = LQ_CREATE_VIEW(Project, Profile) {
        // auto profile = LQ_PARSE_MODEL(Profile);
        LQViewable accueil;
        return accueil;
    });

    // LQViewable Accueil = createView("accueil", Project, Profile)
    //         LQViewable accueil;
    //         return accueil;
    //     });
    return 0;
}

int main() {
    glfwInit();
    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    LQViewable app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0x202225);
    // LQViewable app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0x21252b);
    // LQViewable app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0x282e34);
    // LQViewable app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff);

    LQViewable *parent, *prev;
    createTree(app, parent, prev)
    // navbar
    .add<LQViewable>(0.0f, 0.0f, SCREEN_WIDTH, 1.5_wu, 0x282e34).sub()
        .add<LQViewable>( 0.0f, 0.0f, 2_wu, 1.5_wu, 0x465bb8)
        .add<LQViewable>( 2_wu, 0.0f, 2_wu, 1.5_wu, 0x4e61b5)
        .add<LQViewable>( 4_wu, 0.0f, 2_wu, 1.5_wu, 0x5667b3)
        .add<LQViewable>( 6_wu, 0.0f, 2_wu, 1.5_wu, 0x46538c)
        .add<LQViewable>( 8_wu, 0.0f, 2_wu, 1.5_wu, 0x515c8c)
        .add<LQViewable>(10_wu, 0.0f, 2_wu, 1.5_wu, 0x596494)
        .add<LQViewable>(12_wu, 0.0f, 2_wu, 1.5_wu, 0x5f6994)
        .add<LQViewable>(15_wu, 0.0f, 2_wu, 1.5_wu, 0x636c94)
        .add<LQViewable>(17_wu, 0.0f, 2_wu, 1.5_wu, 0x687094).super()
    // news title
    .add<LQViewable>(1_wu, 2_wu, 13_wu, 1_wu, FVO_GREY)
    // new projects
    .add<LQViewable>(1_wu, 4_wu, 6_wu, 5_wu, FVO_GREY).sub()
        .add<LQViewable>(0.5_wu, 0.5_wu, 5_wu, 4_wu, FVO_GREY).super()
    // soon
    .add<LQViewable>(8_wu, 4_wu, 6_wu, 5_wu, FVO_GREY).sub()
        .add<LQViewable>(0.5_wu, 0.5_wu, 2_wu, 4_wu, FVO_GREY)
        .add<LQViewable>(3.5_wu, 0.5_wu, 2_wu, 4_wu, FVO_GREY).super()
    // lancement rapide
    .add<LQViewable>(15_wu, 2_wu, 4_wu, 9_wu, FVO_GREY).sub()
        .add<LQViewable>(0.5_wu, 1_wu, 3_wu, 1_wu, FVO_GREY)
        .add<LQViewable>(0.5_wu, 3_wu, 3_wu, 1_wu, FVO_GREY)
        .add<LQViewable>(0.5_wu, 5_wu, 3_wu, 2_wu, FVO_GREY).super()
    // news
    .add<LQViewable>(1_wu, 10_wu, 13_wu, 2_wu, FVO_GREY).sub()
        .add<LQViewable>( 0.5_wu, 0.5_wu, 9.5_wu, 1_wu, FVO_GREY)
        .add<LQViewable>(10.5_wu, 0.5_wu,   2_wu, 1_wu, FVO_GREY);

    LQFont font("Montserrat-Regular");
    LQSurface text = font.renderText(U"éèsêà", FVO_TEXT);
    app.appendChild(text);

    // app.drawChildren();
    auto s = "ca marche\n\0plutot bien";
    char* data = new char[27];
    int a = 123456789;

    memcpy(data, &a, 4);
    memcpy(&data[4], s, 23);

    LQRawData lq_rawData(data, 27);
    auto n   = LQ_PARSE(int);
    auto oui = LQ_PARSE(char*);
    auto non = LQ_PARSE(char*);

    std::cout << n << '\n' << oui << non << std::endl;

    // while (window.alive())  {
    //     window.clear(); 
    //     window.blit(app);
    //     // window.blit(text);
    //     window.update();
    // }

    return EXIT_SUCCESS;
}
