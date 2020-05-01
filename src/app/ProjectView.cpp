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
             std::move(width), std::move(height), 0xE9E9E9),
  m_id(project->id)
{
    LQViewable *parent, *prev;
    createTree(*this, parent, prev)
    .add<IMG>(1_em, 1_em, 3_em, 3_em, 0xE9E9E9, "defaultpp.png")
    .add<LQText>(project->nom, prev->right()+20_px, 3_em, 1_em, 0x595959)
    .add<LQText>(project->tag, 0.35f * parent->width(), 3_em, 1_em, 0x356b34)
    .add<LQText>(project->auteur, 0.55f * parent->width(), 3_em, 1_em, 0x595959)
    .add<LQButton>(parent->width()-4_em, 1_em, 3_em, 3_em,
                   0xE9E9E9, "download-icon.png",
        [this]() {
            LQAppController::pushEvent(new dlProjectEvent(m_id));
        });
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

void UL_Project::searchCallback(std::vector<Project*>& projects) {
    auto* prev = firstChild()->nextSibling();
    if (prev) {
        for (auto child = prev->nextSibling();
            child != nullptr;
            child = child->nextSibling())
        {
            delete prev;
            prev = child;
        }
        delete prev;
    }
    m_lastChild = m_firstChild;
    m_childrenCount = 1;
    m_firstChild->setNextSibling(nullptr);
    height() = 1_px;
    if (projects.empty()) {
        auto* first = static_cast<LQViewable*>(firstChild());
        auto* bar = new LQViewable(first->left(), first->bottom()+1_px,
                                   first->width(), first->height(), 0xE9E9E9);
        auto* text = new LQText("AUCUN RÉSULTAT", 0.0f, 3_em, 1_em, 0xc93838);

        text->x() = bar->width() / 2 - text->widthF() / 2;
        bar->appendChild(text);
        appendChild(bar);
    }

    for (auto project : projects) {
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
    .add<LQText>("Trié par :", 0.0f, 12.0f, 1_em, 0x595959)
    .add<LQText>("plus ancien", prev->right()+10_px, 12.0f, prev->height(), 0x356b34)
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
: LQViewport(std::move(x), std::move(y), std::move(w), std::move(h), color)
{
    appendChild(new LQDocument(color));
    // static_cast<LQViewable*>(firstChild())->displayFlex();
    LQViewable *parent, *prev;
    createTree(*static_cast<LQViewable*>(firstChild()), parent, prev)
        .add<LQViewable>(0_px, 0_px, width(), 1_px)
        .add<DIV_Sorting>(25_px, 40_px)
        .add<LQButton>(width()-100_px, 25_px,
                    50_px, 50_px, 0xD9D9D9, "list-view-icon.png")
        .add<LQButton>(prev->left()-75_px, prev->top(),
                    prev->width(), prev->height(), 0xD9D9D9, "grid-view-icon.png")
        // search bar
        .add<LQViewable>(prev->left()-525_px, prev->top(), 500_px, prev->height(), 0xE9E9E9).sub()
            .add<IMG>(12.5f, 12.5f, 25.0f, 25.0f, 0xE9E9E9, "search-icon.png")
            .add<LQTextArea>(prev->right()+12_px, 0.0f, parent->width()-25_px,
                             parent->height(), 0xE9E9E9, "Rechercher...").super()
        .add<UL_Project>(0.15f * width(), 150_px, 0.70f * width());

    static_cast<LQViewable*>(firstChild()->firstChild())->hide();

    auto* textArea = 
        firstChild()->lastChild()->prevSibling()->lastChild();
    static_cast<LQTextArea*>(textArea)->setCallback(
    [this](const std::string& input) {
        static_cast<LQViewable*>(firstChild())->y() = 0.0f;
        LQAppController::resetMousePosition();

        auto* ul =
            static_cast<UL_Project*>(firstChild()->lastChild());
        if (input.empty()) {
            LQAppModel::itemQuery<Project, UL_Project, UL_Project::searchCallback>(
                "project", ul);
        }
        else {
            LQAppModel::itemQuery<Project, UL_Project, UL_Project::searchCallback>(
                "project", ul,
                [input](Project* project) {
                    return project->nom.find(input) != std::string::npos;
                });
        }
    });
    recalc();
};
