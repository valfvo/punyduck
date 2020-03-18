#include <iostream>

#include <forward_list>
#include <vector>

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

// struct LQMetricData {
//     LQMetricData() : data(0) { }
//     int data;

//     LQMetricData operator+(float constant) {
//         std::cout << "data + " << constant << std::endl;
//         return *this;
//     }
//     LQMetricData operator+(LQMetricData variable) {
//         std::cout << "data + variable" << std::endl;
//         return *this;
//     }
//     LQMetricData operator-(float constant) {
//         std::cout << "data - " << constant << std::endl;
//         return *this;
//     }
//     LQMetricData operator-(LQMetricData variable) {
//         std::cout << "data - variable" << std::endl;
//         return *this;
//     }
//     LQMetricData operator*(float constant) {
//         std::cout << "data * " << constant << std::endl;
//         return *this;
//     }
// };

// LQMetricData operator+(float constant, LQMetricData variable) {
//     std::cout << constant << " + data " << std::endl;
//     return variable;
// }

// LQMetricData operator*(float constant, LQMetricData variable) {
//     std::cout << constant << " * data " << std::endl;
//     return variable;
// }

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

    #define printViewable(viewable)\
        std::cout << #viewable << ' ' << &viewable\
                  << " \nx: " << viewable.getX()\
                  << ", y: " << viewable.getY()\
                  << ", width: " << viewable.getWidth()\
                  << ", height: " << viewable.getHeight() << "\n\n"

    #define printViewables()\
        printViewable(viewable1);\
        printViewable(viewable2);\
        printViewable(viewable3);\
        std::cout << "=============================================\n\n"

    // LQMetricData m, o, p, k, l;
    // 2.0f*m + 10.0f + o + 5.0f*p + k + 3.0f * (l - 5.0f);
    // 2.0f*m + 10.0f + o + 5.0f*p + k + 3.0f;
    // 2.0f*m + 10.0f + o + 5.0f*p + k + 3.0f;
    // 3.0f * 5.0f * m;
    // std::forward_list<LQMetricData*> li;

    LQViewable viewable1(0.0f, 0.0f, 700.0f, 700.0f);
    LQViewable viewable2(0.0f, 50.0f, 200.0f, 150.0f);
    LQViewable viewable3(0.0f, 0.0f, 1.0f, 50.0f);
    // LQViewable viewable4(0.0f, 0.0f, 1.0f, 1.0f);
    printViewables();
    // viewable2.width() = viewable1.width() * 0.5f;
    // viewable2.width() = viewable1.width() * 0.5f;
    // printViewables();
    // viewable1.width() = 1000.0f;

    viewable2.right() = viewable1.right();
    viewable3.width() = viewable1.width() / 10.0f;
    viewable3.left() = viewable2.left() - 50.0f;
    printViewables();

    viewable1.width() = 1200.0f;
    printViewables();
    // change viewable2.left() & viewable3.width() & viewable3.left();

    // while (window.alive())  {
    //     window.clear(); 
    //     window.blit(app);
    //     window.update();
    // }

    return EXIT_SUCCESS;
}
