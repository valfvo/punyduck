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
             std::move(width), std::move(height), 0x00ff00)
{
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    // .add<LQImage>(0.0f, 0.0f, parent->height(), parent->height(), project->img)
    // ;
    // .add<LQText>(project->nom, prev->right()+1_wu, 0.0f)
    .add<LQText>(project->nom, 0.0f, 0.0f, 1_em, 0xff0000)
    .add<LQText>(project->tag, prev->right()+1_wu, 0.0f, 1_em, 0x0000ff);
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
: LQViewable(std::move(x), std::move(y))
{
    // appendChild(new LQViewable(0.0f, 0.0f, 0.70f * 1280.0f, 5_em, 0xE9E9E9));
    setClearColor(0xb5b3b3);
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    .add<LQViewable>(0.0f, 0.0f, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34).super()
    .add<LQViewable>(0.0f, prev->bottom()+1_px, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34).super()
    .add<LQViewable>(0.0f, prev->bottom()+1_px, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34).super()
    .add<LQViewable>(0.0f, prev->bottom()+1_px, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34).super()
    .add<LQViewable>(0.0f, prev->bottom()+1_px, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34).super()
    .add<LQViewable>(0.0f, prev->bottom()+1_px, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34).super()
    .add<LQViewable>(0.0f, prev->bottom()+1_px, 0.70f * 1280.0f, 5_em, 0xE9E9E9).sub()
        .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
        .add<LQText>("Darwin's Nightmare", prev->right()+20_px, 3_em, 1_em, 0x595959)
        .add<LQText>("jeu", prev->right()+20_px, 3_em, 1_em, 0x356b34);
    // appendChild(new LQViewable(0.0f, -1_hu, 6_hu, 1_hu));
    // static_cast<LQViewable*>(firstChild())->hide();

    LQ_FOR_EACH(Project, addProject);
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
    // #6D6D6D
    // auto logo = LQAppModel::getFirst("logo");
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    .add<IMG>(12.5f, 12.5f, 25.0f, 25.0f, 0xE9E9E9, "search-icon.png")
    .add<LQText>("Rechercher...", prev->right()+12_px, 30_px, 1_em, 0x808080);
    // .add<LQButton>(prev->right()+0.5_wu, 0.0f, parent->height(), parent->height());
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
    .add<SearchBar>(prev->left()-525_px, prev->top(), 500_px, prev->height())
    .add<UL_Project>(0.15f * 1280.0f, 150_px);
};
