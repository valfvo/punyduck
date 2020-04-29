#include <litequarks/litequarks.hpp>
#include "Components.hpp"

using namespace LQUnit;

IMG::IMG(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
        GLint color, const std::string& path)
: LQViewable(std::move(x), std::move(y),
             std::move(width), std::move(height), color, path) { }

void IMG::draw() { }

LI_Project::LI_Project(const Project* project, LQNumber&& x, LQNumber&& y,
                       LQNumber&& width, LQNumber&& height)
: LQViewable(std::move(x), std::move(y),
             std::move(width), std::move(height), 0xE9E9E9)
{
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    // .add<LQImage>(0.0f, 0.0f, parent->height(), parent->height(), project->img)
    // ;
    // .add<LQText>(project->nom, prev->right()+1_wu, 0.0f)
    .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
    .add<LQText>(project->nom, prev->right()+20_px, 3_em, 1_em, 0x595959)
    .add<LQText>(project->tag, prev->right()+20_px, 3_em, 1_em, 0x356b34);

    // .add<LQText>(project->nom, 0.0f, 0.0f, 1_em, 0xff0000)
    // .add<LQText>(project->tag, prev->right()+1_wu, 0.0f, 1_em, 0x0000ff);
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


UL_Project::UL_Project(LQNumber&& x, LQNumber&& y, LQNumber&& width)
: LQViewable(std::move(x), std::move(y))
{
    setClearColor(0xb5b3b3);
    appendChild(new LQViewable(0.0f, -5_em-1_px, std::move(width), 5_em, 0xE9E9E9));
    static_cast<LQViewable*>(m_firstChild)->hide();

    LQ_FOR_EACH(Project, addProject);
    // lqOn<LQEvent>(this, callback);
}

void UL_Project::addProject(Project* project) {
    auto prev = static_cast<LI_Project*>(lastChild());
    appendChild(new LI_Project(project, prev->left(), prev->bottom()+1_px,
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

#include <iostream>
void UL_Project::searchCallback(std::vector<Project*>& projects) {
    // std::cout << "enter!!! "<< firstChild() << std::endl;
    // std::cout << "this " << this << std::endl;
    // std::cout << firstChild() << "<- first" << std::endl;
    auto prev = firstChild()->nextSibling();
    // std::cout << "erlkghj!!!" << std::endl;
    if (prev) {
        // std::cout << "non!!!" << std::endl;
        for (auto child = prev->nextSibling();
            child != nullptr;
            child = child->nextSibling())
        {
            // std::cout << "nonwerlkgjhb!!!" << std::endl;
            delete prev;
            prev = child;
        }
        delete prev;
    }
    m_lastChild = m_firstChild;
    m_childrenCount = 1;
    m_firstChild->setNextSibling(nullptr);
    // std::cout << "sortie" << std::endl;
    for (auto project : projects) {
    // std::cout << "purodjekto" << std::endl;
        addProject(project);
    }
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


DIV_Sorting::DIV_Sorting(LQNumber&& x, LQNumber&& y)
: LQViewable(std::move(x), std::move(y))
{
    setClearColor(0xD9D9D9);
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    .add<LQText>("Trier par :", 0.0f, 12.0f, 1_em, 0x595959)
    .add<LQText>("plus récent", prev->right()+10_px, 12.0f, prev->height(), 0x356b34)
    .add<LQText>("Filtres :", prev->right()+25_px, 12.0f, prev->height(), 0x595959)
    .add<LQText>("tout", prev->right()+10_px, 12.0f, prev->height(), 0x356b34);
}

SearchBar::SearchBar(LQNumber&& _x, LQNumber&& _y,
                     LQNumber&& _width, LQNumber&& _height)
: LQViewable(std::move(_x), std::move(_y),
             std::move(_width), std::move(_height), 0xE9E9E9)  // 0xECECEC
{
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    .add<IMG>(12.5f, 12.5f, 25.0f, 25.0f, 0xE9E9E9, "search-icon.png")
    .add<LQTextArea>(prev->right()+12_px, 0.0f, parent->width()-25_px,
                     parent->height(), 0xE9E9E9, "Rechercher...");
}


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
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    .add<DIV_Sorting>(25_px, 40_px)
    .add<LQButton>(parent->width()-100_px, 25_px,
                   50_px, 50_px, "list-view-icon.png")
    .add<LQButton>(prev->left()-75_px, prev->top(),
                   prev->width(), prev->height(), "grid-view-icon.png")
    // search bar
    .add<LQViewable>(prev->left()-525_px, prev->top(), 500_px, prev->height(), 0xE9E9E9).sub()
        .add<IMG>(12.5f, 12.5f, 25.0f, 25.0f, 0xE9E9E9, "search-icon.png")
        .add<LQTextArea>(prev->right()+12_px, 0.0f, parent->width()-25_px,
                         parent->height(), 0xE9E9E9, "Rechercher...").super()
    .add<UL_Project>(0.15f * parent->width(), 150_px, 0.70f * parent->width());

    static_cast<LQTextArea*>(lastChild()->prevSibling()->lastChild())->setCallback(
    [this](const std::string& input) {
        if (input.empty()) {
            LQAppModel::itemQuery<Project, UL_Project, UL_Project::searchCallback>(
                "project", static_cast<UL_Project*>(lastChild()));
        }
        else {
            LQAppModel::itemQuery<Project, UL_Project, UL_Project::searchCallback>(
                "project", static_cast<UL_Project*>(lastChild()),
                [input](Project* project) {
                    return project->nom.find(input) != std::string::npos;
                });
        }
    });
};
