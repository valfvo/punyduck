#include <litequarks/litequarks.hpp>
#include "Components.hpp"

using namespace LQUnit;

LI_Project::LI_Project(const Project* project, LQNumber&& x, LQNumber&& y,
                       LQNumber&& width, LQNumber&& height)
: LQViewable(std::move(x), std::move(y), std::move(width), std::move(height))
{
    // LQViewable *parent, *prev;
    // createTree(*this, parent, prev)
    // .add<LQImage>(0.0f, 0.0f, parent->height(), parent->height(), project->img)
    // ;
    // .add<LQText>(project->name, prev->right()+1_wu, 0.0f)
    // .add<LQText>(project->desc, prev->right()+2_wu, 0.0f);
}

void LI_Project::toggleGridView(GLfloat x, GLfloat y) {
    // moveTo(x, y);
    // auto image = static_cast<LQImage*>(firstChild());
    // image->resize(4.5_wu, 4.5_hu);
    // auto titre = static_cast<LQText*>(image->nextSibling());
    // titre->centerX(image->width()/2);
    // static_cast<LQText*>(titre->nextSibling())->hide();
    // resize(image->width(), image->height());
}

void LI_Project::toggleListView(GLfloat y, GLfloat width, GLfloat height) {
    // moveTo(0.0f, y);
    // auto image = static_cast<LQImage*>(firstChild());
    // image->resize(height, height);
    // auto titre = static_cast<LQText*>(image->nextSibling());
    // titre->left = image->right() + 1_wu;
    // static_cast<LQText*>(titre->nextSibling())->display();
    // resize(width, height);
}


UL_Project::UL_Project(LQNumber&& x, LQNumber&& y)
: LQViewable(std::move(x), std::move(y), 6_hu, 1_hu)
{
    appendChild(new LQViewable(0.0f, 0.0f, width(), 1_hu, 0x2f3136));
    // static_cast<LQViewable*>(firstChild())->hide();

    LQ_FOR_EACH(Project, addProject);
    // fitChildrenDimensions();
}

void UL_Project::addProject(Project* project) {
    auto prev = static_cast<LI_Project*>(lastChild());
    appendChild(new LI_Project(project, prev->left(), prev->bottom(),
                                        prev->width(), prev->height()));
}

void UL_Project::toggleGridView() {
    // GLfloat x = 0.0f;
    // GLfloat y = 0.0f;
    // for (auto project = static_cast<LI_Project*>(firstChild()->nextSibling());
    //     project; project = static_cast<LI_Project*>(project->nextSibling()))
    // {
    //     project->toggleGridView(x, y);
    //     x += 4.5_wu;
    //     if (x + 4.5_wu > width()) {
    //         x = 0.0f;
    //         y += 4.5_hu;
    //     }
    // }
}

void UL_Project::toggleListView() {
    // GLfloat y = 0.0f;
    // auto prev = static_cast<LQViewable*>(firstChild());
    // for (auto project = static_cast<LI_Project*>(prev->nextSibling());
    //     project; project = static_cast<LI_Project*>(project->nextSibling()))
    // {
    //     project->toggleListView(y, prev->width(), prev->height());
    //     y += 1_hu;
    // }
}


// UL_Trie::UL_Trie(GLfloat x, GLfloat y)
// : LQViewable(x, y)
// {
//     LQViewable *parent, *prev;
//     createTree(*this, parent, prev)
//     .add<LQButton>("Plus récents", 0.0f, 0.0f)
//     .add<LQButton>("Plus anciens", 0.0f, prev->bottom()+0.5_wu)
//     .add<LQButton>("Mieux notés ", 0.0f, prev->bottom()+0.5_wu);

//     fitChildrenDimensions();
// }


// Trie::Trie(GLfloat x, GLfloat y)
// : LQViewable(x, y)
// {
//     LQViewable *parent, *prev;
//     createTree(*this, parent, prev)
//     .add<LQText>("Trier par :", 0.0f, 0.0f)
//     .add<UL_Trie>(prev->right()+0.5_wu, 0.0f);
// }


// SearchBar::SearchBar(GLfloat x, GLfloat y)
// : LQViewable(x, y)
// {
//     auto logo = LQAppModel::getFirst("logo");
//     LQViewable *parent, *prev;
//     createTree(*this, parent, prev)
//     .add<LQImage>(0.0f, 0.0f, parent->height(), parent->height(), logo->search)
//     .add<LQButton>(prev->right()+0.5_wu, 0.0f, parent->height(), parent->height());
// }


// ButtonMosaique::ButtonMosaique(GLfloat x, GLfloat y, UL_Project* ul)
// : LQViewable(x, y, LQAppModel::getFirst("logo")->mosaique), m_ul(ul)
// {
//     lqOn("click", afficheMosaique);
// }

// void ButtonMosaique::afficheMosaique(LQClickEvent& event) {
//     m_ul->afficheMosaique();
// }


// ButtonListe::ButtonListe(GLfloat x, GLfloat y, UL_Project* ul)
// : LQViewable(x, y, LQAppModel::getFirst("logo")->liste), m_ul(ul)
// {
//     lqOn("click", afficheListe);
// }

// void ButtonListe::afficheListe(LQClickEvent& event) {
//     m_ul->afficheListe();
// }

ProjectView::ProjectView(LQNumber&& x, LQNumber&& y, LQNumber&& w, LQNumber&& h,
                         GLint color)
: LQViewable(std::move(x), std::move(y), std::move(w), std::move(h), color)
{
    // LQViewable *parent, *prev;
    // createTree(*this, parent, prev)
    // // .add<Trie>(1_wu, 2_hu)
    // // .add<UL_Trie>(prev->right(), prev->top())
    // // .add<SearchBar>(prev->right()+9_wu, prev->top())
    // // .add<ButtonMosaique>(prev->right()+0.5_wu, prev->top()) //link avec UL_Project
    // // .add<ButtonListe>(prev->right()+0.5_wu, prev->top())
    // .add<UL_Project>(1_wu, 4_hu);
};
