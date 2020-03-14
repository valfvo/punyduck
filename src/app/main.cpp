#include <iostream>
#include <litequarks/litequarks.hpp>

#define LQwu 50.0f
#define LQhu 50.0f

namespace LQUnit {
    GLfloat operator "" _wu(long double unit) {
        return unit * LQwu;
    }

    GLfloat operator "" _wu(unsigned long long unit) {
        return unit * LQwu;
    }

    GLfloat operator "" _hu(long double unit) {
        return unit * LQhu;
    }

    GLfloat operator "" _hu(unsigned long long unit) {
        return unit * LQhu;
    }
}

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
    LQSurface app(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0x282e34);

    app
    // navbar
    .appendChild(new LQSurface(0.0f, 0.0f, SCREEN_WIDTH, 1.5_wu, 0x282e34), LQ_RETURN_CHILD)
        .appendChild(new LQSurface(     0.0f, 0.0f, 2_wu, 1.5_wu, 0x465bb8))
        .appendChild(new LQSurface(  2_wu, 0.0f, 2_wu, 1.5_wu, 0x4e61b5))
        .appendChild(new LQSurface(2*2_wu, 0.0f, 2_wu, 1.5_wu, 0x5667b3))
        .appendChild(new LQSurface(3*2_wu, 0.0f, 2_wu, 1.5_wu, 0x46538c))
        .appendChild(new LQSurface(4*2_wu, 0.0f, 2_wu, 1.5_wu, 0x515c8c))
        .appendChild(new LQSurface(5*2_wu, 0.0f, 2_wu, 1.5_wu, 0x596494))
        .appendChild(new LQSurface(6*2_wu, 0.0f, 2_wu, 1.5_wu, 0x5f6994))
        .appendChild(new LQSurface(15_wu, 0.0f, 2_wu, 1.5_wu, 0x636c94))
        .appendChild(new LQSurface(17_wu, 0.0f, 2_wu, 1.5_wu, 0x687094), LQ_RETURN_PARENT)
    // news title
    .appendChild(new LQSurface(1_wu, 2_wu, 13_wu, 1_wu, 0xe645e6))
    // new projects
    .appendChild(new LQSurface(1_wu, 4_wu, 6_wu, 5_wu, 0xd61ad6), LQ_RETURN_CHILD)
        .appendChild(new LQSurface(0.5_wu, 0.5_wu, 5_wu, 4_wu, 0xd470d4), LQ_RETURN_PARENT)
    // soon
    .appendChild(new LQSurface(8_wu, 4_wu, 6_wu, 5_wu, 0xd61ad6), LQ_RETURN_CHILD)
        .appendChild(new LQSurface(0.5_wu, 0.5_wu, 2_wu, 4_wu, 0xd470d4))
        .appendChild(new LQSurface(3.5_wu, 0.5_wu, 2_wu, 4_wu, 0xd470d4), LQ_RETURN_PARENT)
    // lancement rapide
    .appendChild(new LQSurface(15_wu, 2_wu, 4_wu, 9_wu, 0xd61ad6), LQ_RETURN_CHILD)
        .appendChild(new LQSurface(0.5_wu, 1_wu, 3_wu, 1_wu, 0xd470d4))
        .appendChild(new LQSurface(0.5_wu, 3_wu, 3_wu, 1_wu, 0xd470d4))
        .appendChild(new LQSurface(0.5_wu, 5_wu, 3_wu, 2_wu, 0xd470d4), LQ_RETURN_PARENT)
    // news
    .appendChild(new LQSurface(1_wu, 10_wu, 13_wu, 2_wu, 0xd61ad6), LQ_RETURN_CHILD)
        .appendChild(new LQSurface(0.5_wu, 0.5_wu, 9.5_wu, 1_wu, 0xd470d4))
        .appendChild(new LQSurface(10.5_wu, 0.5_wu, 2_wu, 1_wu, 0xd470d4));

    app.drawChildren();
    // nav
    // .appendChild(new LQSurface( 0.0f, 12.5f, 25.0f, 25.0f, 0xFF0000))
    // .appendChild(new LQSurface(35.0f, 12.5f, 25.0f, 25.0f, 0x00FF00))
    // .appendChild(new LQSurface(70.0f, 12.5f, 25.0f, 25.0f, 0x0000FF));

    while (window.alive())  {
        window.clear(); 
        window.blit(app);
        window.update();
    }

    return EXIT_SUCCESS;
}
