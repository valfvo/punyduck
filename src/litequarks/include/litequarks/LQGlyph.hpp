#pragma once

#include <glad/glad.h>

struct LQGlyph {
    GLuint textureID;
    int    width;
    int    height;
    int    left;
    int    top;
    GLuint advance;
};
