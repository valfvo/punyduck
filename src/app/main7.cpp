#include <iostream>
#include <vector>
#include <litequarks/litequarks.hpp>

int SCREEN_WIDTH  = 1280;
int SCREEN_HEIGHT = 720;

// #define LQ_TREE_INIT(root)\
//     LQViewable *parent, *prev;\
//     auto tree = createTree(root, parent, prev)

// #define LQ_TREE_INIT(root)\
//     LQTreeCreator((LQViewable*)&root, (LQViewable**)nullptr, (LQViewable**)nullptr);\
//     LQViewable *parent, *prev;\
//     // auto tree = createTree(root, parent, prev)

// #define LQ_TREE_INIT_WITH_FIRST(root, args...)\
//     LQViewable* parent;\
//     LQViewable* prev = new LQViewable(args);\
//     auto tree = createTree(root, parent, prev)

// #define LQ_TREE_PREV_LEFT_BOTTOM_WIDTH_HEIGHT\
//     prev->left(), prev->bottom(), prev->width(), prev->height()

// #define LQ_TREE_PREV_L_B_W_H LQ_TREE_PREV_LEFT_BOTTOM_WIDTH_HEIGHT

// #define LQ_MODEL_LIST(name)\
//     std::forward_list<name##Model*> name##s

// #define LQ_FOR_EACH(item)\
//     for (auto item : item##s)

// #define LQ_VIEWABLE_PARAMS\
//     GLfloat x=0.0f, GLfloat y=0.f,\
//     GLfloat width=0.0f, GLfloat height=0.0f,\
//     GLint color=0x000000

// #define LQ_VIEWABLE_ARGS\
//     x, y, width, height, color

// #define LQ_MODEL(name)\
//     name##Model* name

// using LQImage = LQViewable;
// using LQView = LQViewable;
class LQText;

struct ProjectModel;

// struct ProjectModel {
//     char* titre;
//     char* desc;
//     char* image;
// };

// struct ProfileModel {
//     int   n;
//     char* name;
//     char* bio;
//     int   age;
// };

// class LI_Project : public LQViewable {
// public:
//     LI_Project(const ProjectModel* project, LQ_VIEWABLE_PARAMS)
//     : LQViewable(LQ_VIEWABLE_ARGS)
//     {
//         LQ_TREE_INIT(*this)
//         .add<LQImage>(0.0f, 0.0f, parent->height(), parent->height(), project->image)
//         .add<LQText>(project->titre, prev->right()+1_wu, 0.0f)
//         .add<LQText>(project->desc, prev->right()+2_wu, 0.0f);

//         // fitChildrenDimensions();
//     }
// };

class LQAppController {
public:
    LQAppController() {
        // listen('modelupdate', this);
    }

    static void pollEvents() {
        for (LQEvent event : eventQueue) {
            for (callback : callbacks[event.name]) {
                viewable->callback(event);
            }
        }
    }

    void modelDataUpdated(LQModelDataEvent event) {
        for (auto viewable : s_modelToVieawables[event.model]) {
            for (/*pour chaque nouveau projet*/) {
                viewable->forEach(projet);
            }
        }
    }

    static std::queue<LQEvent> eventQueue;
    static std::map<std::string, std::vector<LQViewable*>> ewd;
    static std::map<std::string, std::vector<LQViewable*, callback>>;
};

void lqInitForEach(std::string model, LQViewable* viewable) {
    AppController::s_modelToViewables[model].push_back(viewable);
}

class UL_Project : public LQViewable {
public:
    UL_Project(GLfloat x, GLfloat y)
    : LQViewable(x, y, 0.0f, 0.0f)
    // : LQViewable(x, y)
    {
        lqInitForEach("Project", this);
        // listen("click") {
        //     AppController::eventToCallbacks.push_back(this, LQViewable::onclick);
        // };
    }

    // void forEach(void* model) override {
    void forEach(void* model) {
        addProject(static_cast<ProjectModel*>(model));
    }

    void addProject(ProjectModel* project) {
        // tree.add<LI_Project>(project, LQ_TREE_PREV_L_B_W_H);
        // extendToChildrenDimensions();
    }
};

// class ProjectModel {
//     LQString titre;
//     LQString desc;
//     LQString image;
//     LQNumber
// };

// class ProfileModel {
//     LQString bio;
// };

// LQString {
//     LQViewable** refs;

//     setText(std::string text) {
//         for (int iRef = 0; refs[iRef]; ++i) {
//             auto ref = refs[iRef];
//             ref->updateString(text);
//         }
//     }
// }

int main() {
    // lqViewTemplate("Home", []() {
    //     LQView accueil(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

    //     LQ_TREE_CREATE(accueil)
    //     .add<UL_Project>(1_wu, 4_hu);
    //     // autre composants de l'accueil
    //     // ...

    //     return accueil;
    // });

    return EXIT_SUCCESS;
}
