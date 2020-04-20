#pragma once

#include <litequarks/litequarks.hpp>
#include "model.hpp"

class LI_Project : public LQViewable {
public:
    LI_Project(const Project* project,
               LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height);

    // void toggleListView(GLfloat y, LQMetricExpr<LQViewable>& width, LQMetricExpr<LQViewable>& height);
    void toggleListView(GLfloat y, GLfloat width, GLfloat height);
    void toggleGridView(GLfloat x, GLfloat y);
};

// class LI_ProjectPreview : public LQViewable {
// public:
//     LI_ProjectPreview(const Project* project, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
// };

class UL_Project : public LQViewable {
public:
    UL_Project(LQNumber&& x, LQNumber&& y);
    void addProject(Project* project);
    void toggleListView();
    void toggleGridView();
};

// class UL_Trie : public LQViewable {
// public:
//     UL_Trie(GLfloat x, GLfloat y);
// };

// class Trie : public LQViewable {
// public:
//     Trie(GLfloat x, GLfloat y);
// };

// class SearchBar : public LQViewable {
// public:
//     SearchBar(GLfloat x, GLfloat y);
// };

// class ButtonMosaique : public LQViewable {
// public:
//     ButtonMosaique(GLfloat x, GLfloat y, UL_Project* ul);
//     void toggleGridView(LQClickEvent& event);
// private:
//     UL_Project* m_ul;
// };

// class ButtonListe : public LQViewable {
// public:
//     ButtonListe(GLfloat x, GLfloat y, UL_Project* ul);

//     void toggleListView(LQClickEvent& event);

// private:
//     UL_Project* m_ul;
// };

// class LI_Friend : public LQViewable {
// public: 
//     LI_Friend(const Friends* Friends, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
// };

// class UL_Friends : public LQViewable {
// public:
//     UL_Friends(GLfloat x, GLfloat y, GLfloat width, GLfloat height);

//     void addFriend(Friends* friends);
// };

LQViewable* ProjectView(LQNumber&& x, LQNumber&& y,
                        LQNumber&& width, LQNumber&& height,
                        GLint color=0x36393f);
