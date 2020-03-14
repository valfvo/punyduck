#include <litequarks/litequarks.hpp>

#define LQwu 50.0f
#define LQhu 50.0f

namespace LQUnit {
    constexpr GLfloat operator "" _wu(long double unit) {
        return unit * LQwu;
    }

    constexpr GLfloat operator "" _wu(unsigned long long unit) {
        return unit * LQwu;
    }

    constexpr GLfloat operator "" _hu(long double unit) {
        return unit * LQhu;
    }

    constexpr GLfloat operator "" _hu(unsigned long long unit) {
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
    .append<LQSurface>(0.0f, 0.0f, SCREEN_WIDTH, 1.5_wu, 0x282e34).subTree()
        .append<LQSurface>( 0.0f, 0.0f, 2_wu, 1.5_wu, 0x465bb8)
        .append<LQSurface>( 2_wu, 0.0f, 2_wu, 1.5_wu, 0x4e61b5)
        .append<LQSurface>( 4_wu, 0.0f, 2_wu, 1.5_wu, 0x5667b3)
        .append<LQSurface>( 6_wu, 0.0f, 2_wu, 1.5_wu, 0x46538c)
        .append<LQSurface>( 8_wu, 0.0f, 2_wu, 1.5_wu, 0x515c8c)
        .append<LQSurface>(10_wu, 0.0f, 2_wu, 1.5_wu, 0x596494)
        .append<LQSurface>(12_wu, 0.0f, 2_wu, 1.5_wu, 0x5f6994)
        .append<LQSurface>(15_wu, 0.0f, 2_wu, 1.5_wu, 0x636c94)
        .append<LQSurface>(17_wu, 0.0f, 2_wu, 1.5_wu, 0x687094).superTree()
    // news title
    .append<LQSurface>(1_wu, 2_wu, 13_wu, 1_wu, 0xe645e6)
    // new projects
    .append<LQSurface>(1_wu, 4_wu, 6_wu, 5_wu, 0xd61ad6).subTree()
        .append<LQSurface>(0.5_wu, 0.5_wu, 5_wu, 4_wu, 0xd470d4).superTree()
    // soon
    .append<LQSurface>(8_wu, 4_wu, 6_wu, 5_wu, 0xd61ad6).subTree()
        .append<LQSurface>(0.5_wu, 0.5_wu, 2_wu, 4_wu, 0xd470d4)
        .append<LQSurface>(3.5_wu, 0.5_wu, 2_wu, 4_wu, 0xd470d4).superTree()
    // lancement rapide
    .append<LQSurface>(15_wu, 2_wu, 4_wu, 9_wu, 0xd61ad6).subTree()
        .append<LQSurface>(0.5_wu, 1_wu, 3_wu, 1_wu, 0xd470d4)
        .append<LQSurface>(0.5_wu, 3_wu, 3_wu, 1_wu, 0xd470d4)
        .append<LQSurface>(0.5_wu, 5_wu, 3_wu, 2_wu, 0xd470d4).superTree()
    // news
    .append<LQSurface>(1_wu, 10_wu, 13_wu, 2_wu, 0xd61ad6).subTree()
        .append<LQSurface>( 0.5_wu, 0.5_wu, 9.5_wu, 1_wu, 0xd470d4)
        .append<LQSurface>(10.5_wu, 0.5_wu,   2_wu, 1_wu, 0xd470d4);

    app.drawChildren();

    while (window.alive())  {
        window.clear(); 
        window.blit(app);
        window.update();
    }

    return EXIT_SUCCESS;
}
