#include <iostream>
#include <litequarks/litequarks.hpp>

using namespace LQUnit;

int SCREEN_WIDTH = 19_wu;
int SCREEN_HEIGHT = 12_hu;

LQColor BLACK(0.0f, 0.0f, 0.0f);
LQColor WHITE(1.0f, 1.0f, 1.0f);
LQColor RED(1.0f, 0.0f, 0.0f);
LQColor GREEN(0.0f, 1.0f, 0.0f);
LQColor BLUE(0.0f, 0.0f, 1.0f);

int main() {
    glfwInit();
    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    //LQSurface app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0x282e34);
    LQSurface Accueil(window.left(), window.top(), window.width(), window.height(), 0x282e34);
    LQSurface Projet(window.left(), window.top(), window.width(), window.height(), 0x282e34);
    LQSurface Collection(window.left(), window.top(), window.width(), window.height(), 0x282e34);
    LQSurface Profil(window.left(), window.top(), window.width(), window.height(), 0x282e34);
    LQSurface Communaute(window.left(), window.top(), window.width(), window.height(), 0x282e34);
    LQSurface Depot(window.left(), window.top(), window.width(), window.height(), 0x282e34);

    LQSurface navBar(window.left(), window.top(), window.width(), 1.5_hu, 0x282e34);
    navBar
    .append<LQSurface>( navBar.left(), navBar.top(), 2_wu, navBar.height(), 0x465bb8)
    .append<LQSurface>( navBar.left()+2_wu, navBar.top(), 2_wu, navBar.height(), 0x4e61b5)
    .append<LQSurface>( navBar.left()+4_wu, navBar.top(), 2_wu, navBar.height(), 0x5667b3)
    .append<LQSurface>( navBar.left()+6_wu, navBar.top(), 2_wu, navBar.height(), 0x46538c)
    .append<LQSurface>( navBar.left()+8_wu, navBar.top(), 2_wu, navBar.height(), 0x515c8c)
    .append<LQSurface>(navBar.left()+10_wu, navBar.top(), 2_wu, navBar.height(), 0x596494)
    .append<LQSurface>(navBar.left()+12_wu, navBar.top(), 2_wu, navBar.height(), 0x5f6994)
    .append<LQSurface>(navBar.left()+15_wu, navBar.top(), 2_wu, navBar.height(), 0x636c94)
    .append<LQSurface>(navBar.left()+17_wu, navBar.top(), 2_wu, navBar.height(), 0x687094)
    navBar.drawChildren();

    Accueil
    // navbar
    .appendChild(navBar)
    // .append<LQSurface>(Accueil.left(), Accueil.top(), Accueil.width(), 1.5_hu, 0x282e34).subTree()
    //     .append<LQSurface>( Accueil.left(), Accueil.top(), 2_wu, tree->quark.height, 0x465bb8)
    //     .append<LQSurface>( Accueil.left()+2_wu, Accueil.top(), 2_wu, tree->quark.height, 0x4e61b5)
    //     .append<LQSurface>( Accueil.left()+4_wu, Accueil.top(), 2_wu, tree->quark.height, 0x5667b3)
    //     .append<LQSurface>( Accueil.left()+6_wu, Accueil.top(), 2_wu, tree->quark.height, 0x46538c)
    //     .append<LQSurface>( Accueil.left()+8_wu, Accueil.top(), 2_wu, tree->quark.height, 0x515c8c)
    //     .append<LQSurface>(Accueil.left()+10_wu, Accueil.top(), 2_wu, tree->quark.height, 0x596494)
    //     .append<LQSurface>(Accueil.left()+12_wu, Accueil.top(), 2_wu, tree->quark.height, 0x5f6994)
    //     .append<LQSurface>(Accueil.left()+15_wu, Accueil.top(), 2_wu, tree->quark.height, 0x636c94)
    //     .append<LQSurface>(Accueil.left()+17_wu, Accueil.top(), 2_wu, tree->quark.height, 0x687094).superTree()
    // news title
    .append<LQSurface>(Accueil.left()+1_wu,  Accueil.top()+2_hu, Accueil.width()-6_wu, 1_hu, 0xe645e6)
    // new projects
    .append<LQSurface>(Accueil.left()+1_wu,  Accueil.top()+4_hu, 6_wu, 5_hu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+0.5_wu, tree->quark.top()+0.5_hu, tree->quark.width()-1_wu, tree->quark.height()-1_hu, 0xd470d4).superTree()
    // soon
    .append<LQSurface>(Accueil.left()+8_wu, Accueil.top()+4_hu, 6_wu, 5_hu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+0.5_wu, tree->quark.top()+0.5_hu, 2_wu, 4_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+3.5_wu, tree->quark.top()+0.5_hu, 2_wu, 4_hu, 0xd470d4).superTree()
    // lancement rapide
    .append<LQSurface>(Accueil.left()+15_wu, Accueil.top()+2_hu, 4_wu, 9_wu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+0.5_wu, tree->quark.top()+1_hu, tree->quark.width()-1_wu, 1_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+0.5_wu, tree->quark.top()+3_hu, tree->quark.width()-1_wu, 1_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+0.5_wu, tree->quark.top()+5_hu, tree->quark.width()-1_wu, 2_hu, 0xd470d4).superTree()
    // news
    .append<LQSurface>(Accueil.left()+1_wu,  Accueil.top()+10_hu, Accueil.width()-6_wu, 2_wu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+0.5_wu,  tree->quark.top()+0.5_hu, tree->quark.width()-3.5_wu, 1_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+10.5_wu, tree->quark.top()+0.5_hu,   2_wu, 1_hu, 0xd470d4);
    Accueil.drawChildren();


    Projet
    //navbar
    .appendChild(navBar)
    // .append<LQSurface>(Projet.left(), Projet.top(), Projet.width(), 1.5_hu, 0x282e34).subTree()
    //     .append<LQSurface>( Projet.left(), Projet.top(), 2_wu, tree->quark.height, 0x465bb8)
    //     .append<LQSurface>( Projet.left()+2_wu, Projet.top(), 2_wu, tree->quark.height, 0x4e61b5)
    //     .append<LQSurface>( Projet.left()+4_wu, Projet.top(), 2_wu, tree->quark.height, 0x5667b3)
    //     .append<LQSurface>( Projet.left()+6_wu, Projet.top(), 2_wu, tree->quark.height, 0x46538c)
    //     .append<LQSurface>( Projet.left()+8_wu, Projet.top(), 2_wu, tree->quark.height, 0x515c8c)
    //     .append<LQSurface>(Projet.left()+10_wu, Projet.top(), 2_wu, tree->quark.height, 0x596494)
    //     .append<LQSurface>(Projet.left()+12_wu, Projet.top(), 2_wu, tree->quark.height, 0x5f6994)
    //     .append<LQSurface>(Projet.left()+15_wu, Projet.top(), 2_wu, tree->quark.height, 0x636c94)
    //     .append<LQSurface>(Projet.left()+17_wu, Projet.top(), 2_wu, tree->quark.height, 0x687094).superTree()
    //trie et filtre
    .append<LQSurface>(Projet.left()+1_wu, Projet.top()+2_hu, 7_wu, 1_hu, 0xd61ad6)
    //3 boutons en haut a droite
    .append<LQSurface>(Projet.left()+11.5_wu, Projet.top()+2_hu, 4_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Projet.left()+16_wu, Projet.top()+2_hu, 1_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Projet.left()+17_wu, Projet.top()+2_hu, 1_wu, 1_hu, 0xd61ad6)
    //bars nom projet etc..
    .append<LQSurface>(Projet.left()+1_wu, Projet.top()+4_hu, 15_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Projet.left()+1_wu, Projet.top()+5.5_hu, 15_wu, 1_hu, 0xd61ad6)
    //3 grands carrés
    .append<LQSurface>(Projet.left()+1_wu, Projet.top()+7_hu, 4.5_wu, 4.5_hu, 0xd61ad6)
    .append<LQSurface>(Projet.left()+6.5_wu, Projet.top()+7_hu, 4.5_wu, 4.5_hu, 0xd61ad6)
    .append<LQSurface>(Projet.left()+12_wu, Projet.top()+7_hu, 4.5_wu, 4.5_hu, 0xd61ad6)
    //petit carre sur le grand carre
    .append<LQSurface>(Projet.left()+15_wu, Projet.top()+9_hu, 3_wu, 2.75_hu,  0xd470d4)
    Projet.drawChildren();


    Collection
    //navbar
    .appendChild(navBar)
    // .append<LQSurface>(Collection.left(), Collection.top(), Collection.width(), 1.5_hu, 0x282e34).subTree()
    //     .append<LQSurface>( Collection.left(), Collection.top(), 2_wu, tree->quark.height, 0x465bb8)
    //     .append<LQSurface>( Collection.left()+2_wu, Collection.top(), 2_wu, tree->quark.height, 0x4e61b5)
    //     .append<LQSurface>( Collection.left()+4_wu, Collection.top(), 2_wu, tree->quark.height, 0x5667b3)
    //     .append<LQSurface>( Collection.left()+6_wu, Collection.top(), 2_wu, tree->quark.height, 0x46538c)
    //     .append<LQSurface>( Collection.left()+8_wu, Collection.top(), 2_wu, tree->quark.height, 0x515c8c)
    //     .append<LQSurface>(Collection.left()+10_wu, Collection.top(), 2_wu, tree->quark.height, 0x596494)
    //     .append<LQSurface>(Collection.left()+12_wu, Collection.top(), 2_wu, tree->quark.height, 0x5f6994)
    //     .append<LQSurface>(Collection.left()+15_wu, Collection.top(), 2_wu, tree->quark.height, 0x636c94)
    //     .append<LQSurface>(Collection.left()+17_wu, Collection.top(), 2_wu, tree->quark.height, 0x687094).superTree()
    //trier par
    .append<LQSurface>(Collection.left()+1_wu, Collection.top()+2_hu, 3_wu, 1_hu, 0xd61ad6)
    //3 boutons en haut a droite
    .append<LQSurface>(Collection.left()+11.5_wu, Collection.top()+2_hu, 4_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+16_wu, Collection.top()+2_hu, 1_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+17_wu, Collection.top()+2_hu, 1_wu, 1_hu, 0xd61ad6)
    //3 petits îcones
    .append<LQSurface>(Collection.left()+1_wu, Collection.top()+3.5_hu, 1_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+1_wu, Collection.top()+5_hu, 1_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+1_wu, Collection.top()+5.5_hu, 1_wu, 1_hu, 0xd61ad6)
    //3 bars a cotés de leurs îcones
    .append<LQSurface>(Collection.left()+3_wu, Collection.top()+3.5_hu, 11_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+3_wu, Collection.top()+5_hu, 11_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+3_wu, Collection.top()+5.5_hu, 11_wu, 1_hu, 0xd61ad6)
    //carre en dessous des boutons
    .append<LQSurface>(Collection.left()+15_wu, Collection.top()+3.5_hu, 3_wu, 3.5_hu, 0xd61ad6)
    //3 carres en bas de page
    .append<LQSurface>(Collection.left()+1_wu, Collection.top()+8_hu, 4.5_wu, 3.5_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+6.5_wu, Collection.top()+8_hu, 4.5_wu, 3.5_hu, 0xd61ad6)
    .append<LQSurface>(Collection.left()+12_wu, Collection.top()+8_hu, 4.5_wu, 3.5_hu, 0xd61ad6)
    Collection.drawChildren();


    Profil
    //navbar
    .appendChild(navBar)
    // .append<LQSurface>(Profil.left(), Profil.top(), Profil.width(), 1.5_hu, 0x282e34).subTree()
    //     .append<LQSurface>( Profil.left(), Profil.top(), 2_wu, tree->quark.height, 0x465bb8)
    //     .append<LQSurface>( Profil.left()+2_wu, Profil.top(), 2_wu, tree->quark.height, 0x4e61b5)
    //     .append<LQSurface>( Profil.left()+4_wu, Profil.top(), 2_wu, tree->quark.height, 0x5667b3)
    //     .append<LQSurface>( Profil.left()+6_wu, Profil.top(), 2_wu, tree->quark.height, 0x46538c)
    //     .append<LQSurface>( Profil.left()+8_wu, Profil.top(), 2_wu, tree->quark.height, 0x515c8c)
    //     .append<LQSurface>(Profil.left()+10_wu, Profil.top(), 2_wu, tree->quark.height, 0x596494)
    //     .append<LQSurface>(Profil.left()+12_wu, Profil.top(), 2_wu, tree->quark.height, 0x5f6994)
    //     .append<LQSurface>(Profil.left()+15_wu, Profil.top(), 2_wu, tree->quark.height, 0x636c94)
    //     .append<LQSurface>(Profil.left()+17_wu, Profil.top(), 2_wu, tree->quark.height, 0x687094).superTree()
    //grand carre haut de page
    .append<LQSurface>(Profil.left(), Profil.top()+1.5_hu, profil.width(), 3.5_hu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+7_wu, tree->quark.left()+0.5_hu, 3_wu, 2.5_hu, 0xd470d4).superTree()
    //grand carre bas gauche
    .append<LQSurface>(Profil.left(), Profil.top()+5_hu, 8_wu, 7_hu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+1_wu, tree->quark.top()+1_hu, 6_wu, 1_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+1_wu, tree->quark.top()+2.5_hu, 6_wu, 1_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+1_wu, tree->quark.top()+4_hu, 6_wu, 1_hu, 0xd470d4)
        .append<LQSurface>(tree->quark.left()+1_wu, tree->quark.top()+5.5_hu, 6_wu, 1_hu, 0xd470d4).superTree()
    //grand carre en bas au milieu
    .append<LQSurface>(Profil.left()+8_wu, Profil.top()+5_hu, 7_wu, 7_hu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+4_wu, tree->quark.top()+6_hu, 2.5_wu, 0.75_hu, 0xd470d4).superTree()
    //amis
    .append<LQSurface>(Profil.left()+15_wu, Profil.top()+5_hu, 4_wu, 4_hu, 0xd61ad6)
    //carre en bas a droite
    .append<LQSurface>(Profil.left()+15_wu, Profil.top()+9_hu, 4_wu, 3_hu, 0xd61ad6).subTree()
        .append<LQSurface>(tree->quark.left()+0.5_wu, tree->quark.top()+2_hu, 3_wu, 0.75_hu, 0xd470d4).superTree()
    Profil.drawChildren();
    

    Communaute
    //navbar
    .appendChild(navBar)
    // .append<LQSurface>(Communaute.left(), Communaute.top(), Communaute.width(), 1.5_hu, 0x282e34).subTree()
    //     .append<LQSurface>( Communaute.left(), Communaute.top(), 2_wu, tree->quark.height, 0x465bb8)
    //     .append<LQSurface>( Communaute.left()+2_wu, Communaute.top(), 2_wu, tree->quark.height, 0x4e61b5)
    //     .append<LQSurface>( Communaute.left()+4_wu, Communaute.top(), 2_wu, tree->quark.height, 0x5667b3)
    //     .append<LQSurface>( Communaute.left()+6_wu, Communaute.top(), 2_wu, tree->quark.height, 0x46538c)
    //     .append<LQSurface>( Communaute.left()+8_wu, Communaute.top(), 2_wu, tree->quark.height, 0x515c8c)
    //     .append<LQSurface>(Communaute.left()+10_wu, Communaute.top(), 2_wu, tree->quark.height, 0x596494)
    //     .append<LQSurface>(Communaute.left()+12_wu, Communaute.top(), 2_wu, tree->quark.height, 0x5f6994)
    //     .append<LQSurface>(Communaute.left()+15_wu, Communaute.top(), 2_wu, tree->quark.height, 0x636c94)
    //     .append<LQSurface>(Communaute.left()+17_wu, Communaute.top(), 2_wu, tree->quark.height, 0x687094).superTree()
    //bouton forum
    .append<LQSurface>(Communaute.left()+1_wu, Communaute.top()+2_hu, 3_wu, 1_hu, 0xd61ad6)
    //bouton categorie
    .append<LQSurface>(Communaute.left()+5_wu, Communaute.top()+2_hu, 4_wu, 1_hu, 0xd61ad6)
    //bouton en haut à droite
    .append<LQSurface>(Communaute.left()+15_wu, Communaute.top()+2_hu, 3_wu, 1_hu, 0xd61ad6)
    //sujets
    .append<LQSurface>(Communaute.left()+1_wu, Communaute.top()+4_hu, 4_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Communaute.left()+1_wu, Communaute.top()+5.5_hu, 4_wu, 1_hu, 0xd61ad6)
    .append<LQSurface>(Communaute.left()+1_wu, Communaute.top()+7_hu, 4_wu, 1_hu, 0xd61ad6)
    //sujet3 ouvert
    .append<LQSurface>(Communaute.left()+1_wu, Communaute.top()+8_hu, 17_wu, 3.5_hu, 0xd61ad6)
    Communaute.drawChildren();


    Depot
    //navbar
    .appendChild(navBar)
    .append<LQSurface>(Depot.left(), Depot.top(), Depot.width(), 1.5_hu, 0x282e34).subTree()
        // .append<LQSurface>( Depot.left(), Depot.top(), 2_wu, tree->quark.height, 0x465bb8)
        // .append<LQSurface>( Depot.left()+2_wu, Depot.top(), 2_wu, tree->quark.height, 0x4e61b5)
        // .append<LQSurface>( Depot.left()+4_wu, Depot.top(), 2_wu, tree->quark.height, 0x5667b3)
        // .append<LQSurface>( Depot.left()+6_wu, Depot.top(), 2_wu, tree->quark.height, 0x46538c)
        // .append<LQSurface>( Depot.left()+8_wu, Depot.top(), 2_wu, tree->quark.height, 0x515c8c)
        // .append<LQSurface>(Depot.left()+10_wu, Depot.top(), 2_wu, tree->quark.height, 0x596494)
        // .append<LQSurface>(Depot.left()+12_wu, Depot.top(), 2_wu, tree->quark.height, 0x5f6994)
        // .append<LQSurface>(Depot.left()+15_wu, Depot.top(), 2_wu, tree->quark.height, 0x636c94)
        // .append<LQSurface>(Depot.left()+17_wu, Depot.top(), 2_wu, tree->quark.height, 0x687094).superTree()
    //zone depot
    .append<LQSurface>(Depot.left()+3_wu, Depot.top()+2_hu, 13_wu, 3_hu, 0xd61ad6)
    //titre porjet
    .append<LQSurface>(Depot.left()+6_wu, Depot.top()+6_hu, 7_wu, 1_hu, 0xd61ad6)
    //description
    .append<LQSurface>(Depot.left()+6_wu, Depot.top()+8_hu, 7_wu, 3_hu, 0xd61ad6)
    //bouton de depot
    .append<LQSurface>(Depot.left()+15_wu, Depot.top()+10.5_hu, 3_wu, 1_hu, 0xd61ad6)
    Depot.drawChildren();



    #define printViewable(viewable)\
        std::cout << #viewable << ' ' << &viewable\
                  << " \nx: " << viewable.getX()\
                  << ", y:" << viewable.getY()\
                  << ", width: " << viewable.getWidth()\
                  << ", height:" << viewable.getHeight() << "\n\n"

    #define printViewables()\
        printViewable(viewable1);\
        printViewable(viewable2);\
        printViewable(viewable3);\
        printViewable(viewable4);\
        std::cout << "=============================================\n\n"

    LQViewable viewable1(100.0f, 100.0f, 500.0f, 250.0f);
    LQViewable viewable2(0.0f, 0.0f, 1.0f, 400.0f);
    LQViewable viewable3(0.0f, 0.0f, 1.0f, 1.0f);
    LQViewable viewable4(0.0f, 0.0f, 1.0f, 1.0f);
    printViewables();
    // viewable2.width() = 0.5f * viewable1.width();
    viewable2.width() = viewable1.width() * 0.5f;
    printViewables();
    viewable1.width() = 1000.0f;
    printViewables();
    // viewable2.width() = viewable1.width();

    while (window.alive())  {
        window.clear(); 
        window.blit(Accueil);
        window.update();
    }

    return EXIT_SUCCESS;
}
