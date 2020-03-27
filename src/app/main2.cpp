#include <iostream>
#include <litequarks/litequarks.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace LQUnit;

int SCREEN_WIDTH  = 1280;
int SCREEN_HEIGHT = 720;

LQColor BLACK (0x000000);
LQColor WHITE (0xffffff);
LQColor RED   (0xff0000);
LQColor GREEN (0x00ff00);
LQColor BLUE  (0x0000ff);

#define FVO_GREEN 0x1DBF73
#define FVO_GRAY  0x3F3F3F

int main() {
    glfwInit();
    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    LQViewable app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0x282e34);

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
    .add<LQViewable>(1_wu, 2_wu, 13_wu, 1_wu, 0xe645e6)
    // new projects
    .add<LQViewable>(1_wu, 4_wu, 6_wu, 5_wu, FVO_GRAY).sub()
        .add<LQViewable>(0.5_wu, 0.5_wu, 5_wu, 4_wu, FVO_GREEN).super()
    // soon
    .add<LQViewable>(8_wu, 4_wu, 6_wu, 5_wu, FVO_GRAY).sub()
        .add<LQViewable>(0.5_wu, 0.5_wu, 2_wu, 4_wu, FVO_GREEN)
        .add<LQViewable>(3.5_wu, 0.5_wu, 2_wu, 4_wu, FVO_GREEN).super()
    // lancement rapide
    .add<LQViewable>(15_wu, 2_wu, 4_wu, 9_wu, FVO_GRAY).sub()
        .add<LQViewable>(0.5_wu, 1_wu, 3_wu, 1_wu, FVO_GREEN)
        .add<LQViewable>(0.5_wu, 3_wu, 3_wu, 1_wu, FVO_GREEN)
        .add<LQViewable>(0.5_wu, 5_wu, 3_wu, 2_wu, FVO_GREEN).super()
    // news
    .add<LQViewable>(1_wu, 10_wu, 13_wu, 2_wu, FVO_GRAY).sub()
        .add<LQViewable>( 0.5_wu, 0.5_wu, 9.5_wu, 1_wu, FVO_GREEN)
        .add<LQViewable>(10.5_wu, 0.5_wu,   2_wu, 1_wu, FVO_GREEN);

    app.drawChildren();

    /*
    #define printViewable(viewable)\
        std::cout << #viewable << ' ' << &viewable\
                  << " \nx: " << viewable.getX()\
                  << ", y: " << viewable.getY()\
                  << ", width: " << viewable.getWidth()\
                  << ", height: " << viewable.getHeight() << "\n\n"

    // #define printViewables()\
        printViewable(viewable1);\
        printViewable(viewable2);\
        printViewable(viewable3);\
        std::cout << "=============================================\n\n"
    */

    // LQMetricData m, o, p, k, l;
    // 2.0f*m + 10.0f + o + 5.0f*p + k + 3.0f * (l - 5.0f);
    // 2.0f*m + 10.0f + o + 5.0f*p + k + 3.0f;
    // 2.0f*m + 10.0f + o + 5.0f*p + k + 3.0f;
    // 3.0f * 5.0f * m;
    // std::forward_list<LQMetricData*> li;

    // LQViewable viewable1(0.0f, 0.0f, 700.0f, 700.0f);
    // LQViewable viewable2(0.0f, 50.0f, 200.0f, 150.0f);
    // LQViewable viewable3(0.0f, 0.0f, 1.0f, 50.0f);
    // LQViewable viewable4(0.0f, 0.0f, 1.0f, 1.0f);
    // printViewables();
    // viewable2.width() = viewable1.width() * 0.5f;
    // viewable2.width() = viewable1.width() * 0.5f;
    // printViewables();
    // viewable1.width() = 1000.0f;

    // viewable2.right() = viewable1.right();
    // viewable3.width() = viewable1.width() / 10.0f;
    // viewable3.left() = viewable2.left() - 50.0f;
    // printViewables();

    // viewable1.width() = 1200.0f;
    // printViewables();
    // change viewable2.left() & viewable3.width() & viewable3.left();

    // while (window.alive())  {
    //     window.clear(); 
    //     window.blit(app);
    //     window.update();
    // }

    return EXIT_SUCCESS;
}
