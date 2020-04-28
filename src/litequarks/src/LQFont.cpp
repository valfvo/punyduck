#include <algorithm>  // std::max
#include <iostream>
#include <utility>  // std::make_pair, std::move

#include <litequarks/LQFont.hpp>

FT_Library LQFont::s_library = nullptr;

LQFont::LQFont()
: m_face(nullptr) { }  // doesn't initLibrary to allow static init

LQFont::LQFont(const std::string& fontname, LQNumber&& size)
: LQFont()
{
    // std::string fontpath = "C:/Windows/Fonts/" + fontname + ".ttf";
    std::string fontpath = "fonts/" + fontname + ".ttf";
    if (FT_New_Face(s_library, fontpath.c_str(), 0, &m_face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(m_face, 0, size);
}

LQFont::~LQFont() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(s_library);
}

void LQFont::change(const std::string& fontname, LQNumber&& size) {
    FT_Done_Face(m_face);
    m_glyphs.clear();

    std::string fontpath = "fonts/" + fontname + ".ttf";
    if (FT_New_Face(s_library, fontpath.c_str(), 0, &m_face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(m_face, 0, size);
}

const LQGlyph& LQFont::renderChar(char32_t character) {
    if (m_glyphs.find(character) == m_glyphs.end()) {
        if (FT_Load_Char(m_face, character, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        }
        m_glyphs.insert(std::make_pair(character,
            LQGlyph{
                LQTexture{std::max(m_face->glyph->bitmap.width, 1U),
                        std::max(m_face->glyph->bitmap.rows, 1U),
                        m_face->glyph->bitmap.buffer,
                        GL_RED, 0, GL_RED,
                        GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER},
                m_face->glyph->bitmap_left,
                m_face->glyph->bitmap_top,
                m_face->glyph->advance.x >> 6}));
    }
    return m_glyphs[character];
}

LQText LQFont::renderText(
    const std::u32string& text, GLuint color,
    LQNumber&& x, LQNumber&& y, LQNumber&& size)
{
    LQColor c(color);

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
    const LQGlyph* p_glyph = nullptr;

    GLint ascender = 0;
    GLint descender = 0;

    for (auto c : text) {
        glyphs[iGlyph] = &renderChar(c);
        p_glyph = glyphs[iGlyph];
        width += p_glyph->advanceX;
        ascender = std::max(ascender, p_glyph->top);
        descender = std::max(descender,
                             p_glyph->bitmap.getHeight() - p_glyph->top);
        ++iGlyph;
    }

    LQText renderedText(text, std::move(x), y - ascender,
                        width, ascender + descender, ascender);
    renderedText.setClearColor(c.r(), c.g(), c.b(), 0.0f);
    renderedText.clear();
    renderedText.setShader(new LQShader("shaders/font.vert", "shaders/font.frag"));

    GLfloat advance = 0;
    for (GLuint iGlyph = 0; iGlyph < text.length(); ++iGlyph) {
        p_glyph = glyphs[iGlyph];

        renderedText.blit(
            p_glyph->bitmap,
            advance + p_glyph->left,
            ascender - p_glyph->top,
            VAO);

        advance += p_glyph->advanceX;
    }

    return renderedText;
}

void LQFont::init() {
    if (FT_Init_FreeType(&s_library)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
}
