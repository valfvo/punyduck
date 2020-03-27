#include <litequarks/LQFont.hpp>
#include <iostream>

FT_Library LQFont::s_library = nullptr;

LQFont::LQFont(std::string fontname)
: m_face(nullptr)
{
    if (s_library == nullptr) {
        initLibrary();
    }

    // std::string fontpath = "C:/Windows/Fonts/" + fontname + ".ttf";
    std::string fontpath = "fonts/" + fontname + ".ttf";
    if (FT_New_Face(s_library, fontpath.c_str(), 0, &m_face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(m_face, 0, 100);
}

LQFont::~LQFont() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(s_library);
}

const LQGlyph& LQFont::renderChar(char32_t character) {
    if (m_glyphs.find(character) == m_glyphs.end()) {
        if (FT_Load_Char(m_face, character, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        }
        LQGlyph glyph = {
            LQTexture{m_face->glyph->bitmap.width,
                      m_face->glyph->bitmap.rows,
                      m_face->glyph->bitmap.buffer,
                      GL_RED, 0, GL_RED,
                      GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER},
            m_face->glyph->bitmap_left,
            m_face->glyph->bitmap_top,
            m_face->glyph->advance.x >> 6
        };
        m_glyphs.insert({character, glyph});
    }
    return m_glyphs[character];
}

LQSurface LQFont::renderText(std::u32string text, GLuint color) {
    LQColor c(color);
    // LQColor c(1.0f, 0.0f, 0.0f, 0.5f);
    GLfloat vertices[54] = {
    //  x     y     z       tex coords    color
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   c.r(), c.g(), c.b(), c.a(),  // bottom left
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   c.r(), c.g(), c.b(), c.a(),  // top right
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   c.r(), c.g(), c.b(), c.a(),  // top left

        0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   c.r(), c.g(), c.b(), c.a(),  // bottom left
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   c.r(), c.g(), c.b(), c.a(),  // bottom right
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   c.r(), c.g(), c.b(), c.a()   // top right
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(0);  // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);  // tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);  // color
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const LQGlyph** glyphs = new const LQGlyph*[text.length()];

    GLfloat width = 0;
    LQindex iGlyph = 0;
    for (auto c : text) {
        glyphs[iGlyph] = &renderChar(c);
        width += glyphs[iGlyph]->bitmap.getWidth() + glyphs[iGlyph]->advanceX;
        ++iGlyph;
    }

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LQSurface renderedText(100.0f, 250.0f, width, 100);
    // renderedText.setClearColor(c.r(), c.g(), c.b(), 0.0f);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    renderedText.setClearColor(c.r(), c.g(), c.b(), 0.0f);
    // renderedText.setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    renderedText.clear();
    renderedText.setShader(new LQShader("font.vert", "font.frag"));

    GLfloat x = 0;
    const LQGlyph* p_glyph = nullptr;
    for (GLuint iGlyph = 0; iGlyph < text.length(); ++iGlyph) {
        p_glyph = glyphs[iGlyph];

        renderedText.blit(
            p_glyph->bitmap,
            x + p_glyph->left,
            77 - p_glyph->top,
            VAO);

        x += p_glyph->advanceX;
    }
    // glDisable(GL_BLEND);
    // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return renderedText;
}

void LQFont::initLibrary() {
    if (FT_Init_FreeType(&s_library)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
}
