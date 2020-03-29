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

using LQImage = LQViewable;
using LQView = LQViewable;
class LQText;

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

class BarreProjet : public LQViewable {
public:
    //BarreProjet(ProjectModel* projet, LQ_VIEWABLE_ARGS)
    BarreProjet(const ProjectModel& projet, BarreProjet* prevBarre)
    : LQViewable()
    {
        LQViewable *parent, *prev;
        createTree(*this, parent, prev)
        .add<LQImage>(0.0f, 0.0f, parent->height(), parent->height(), projet.image)
        .add<LQText>(projet.titre, prev->right()+1_wu, 0.0f)
        .add<LQText>(projet.desc, prev->right()+2_wu, 0.0f);

        // fitChildrenDimensions();

        // if (prevBarre == nullptr) {
        //     GLfloat width = 0.0f;
        //     for (auto child = m_firstChild; child; child = child->nextSibling()) {
        //         // width += static_cast<LQViewable*>(child)->width();
        //     }
        //     resizeWidth(width);
        // }
        // else {
        //     // moveTo(prevBarre->left(), prevBarre->bottom()+1_hu);
        //     // resize(prevBarre->width(), prevBarre->height());
        // }
    }
};

//template<class T>
//using LQList = std::forward_list<T>

#define LQ_TREE_INIT()\
    LQViewable *parent, *prev;\
    auto tree = createTree(*this, parent, prev)

#define LQ_TREE_INIT_WITH_FIRST(args...)\
    LQViewable* parent;\
    LQViewable* prev = new LQViewable(args);\
    auto tree = createTree(*this, parent, prev)

#define LQ_TREE_PREV_LEFT_BOTTOM_WIDTH_HEIGHT\
    prev->left(), prev->bottom(), prev->width(), prev->height()

#define LQ_TREE_PREV_L_B_W_H LQ_TREE_PREV_LEFT_BOTTOM_WIDTH_HEIGHT

#define LQ_MODEL_LIST(name)\
    std::forward_list<name##Model> name##s

#define LQ_FOR_EACH(item)\
    for (auto item : item##s)

class ListeProjets : public LQViewable {
public:
    // ListeProjets(LQ_MODEL_LIST(Project) projets, LQ_VIEWABLE_ARGS)
    // ListeProjets(ProjectModel* projets, GLfloat x, GLfloat y)
    ListeProjets(LQ_MODEL_LIST(Project), GLfloat x, GLfloat y)
    : LQViewable(x, y, 0.0f, 0.0f)
    {
        appendChild(new BarreProjet(projets[0], nullptr));
        auto barre = static_cast<BarreProjet*>(m_lastChild);
        // resizeWidth(barre->width());

        int nBarres = 0;
        for (LQindex projet = 0; projet; ++projet) {
            appendChild(new BarreProjet(projets[projet], static_cast<BarreProjet*>(m_lastChild)));
            ++nBarres;
        }

        LQ_TREE_INIT_WITH_FIRST(0.0f, 0.0f, -1.0f, 1.0f);
        LQ_FOR_EACH(Project) {
            tree.add<BarreProjet>(Project, LQ_TREE_PREV_L_B_W_H);
        }

        // fitChildrenDimensions();
    }
};

// LQView createView(std::string name, LQView (*constructor)(void* data)) {
//     return constructor(nullptr);
// }

class LQModelData {

};

// LQView createView(std::string name, const char* models[], LQView (*constructor)(void* data)) {
LQView createView(std::string name, std::vector<std::string> models,
                  LQView (*constructor)(LQModelData data)) {
    LQModelData data;
    return constructor(data);
}

LQView lamb() {
// lamb(LQModelData lq_modelData)
    LQView accueil(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

    LQViewable *parent, *prev;
    createTree(accueil, parent, prev)
    .add<ListeProjets>(nullptr, 1_wu, 4_hu);
    // .add<ListeProjets>(LQ_PARSE_MODEL(Project), 1_wu, 4_hu);
    // .add<BoutonTrie>(parent->firstChild());

    return accueil;
}

int main2() {
    // const char* models[] = {
    //     "oui",
    //     "non"
    // };

    LQViewable Accueil = createView("accueil",
        {"project", "profile"},
        [](LQModelData data) {
            // data.parse("profile");
            LQViewable accueil;
            return accueil;
        });
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

    app.drawChildren();

    while (window.alive())  {
        window.clear(); 
        window.blit(app);
        // window.blit(text);
        window.update();
    }

    return EXIT_SUCCESS;
}
