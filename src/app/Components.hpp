#pragma once

#include <string>

#include <litequarks/litequarks.hpp>
#include "model.hpp"

class IMG : public LQViewable {
public:
    IMG(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
        GLint color, const std::string& path);

    void draw() override;
};

class LI_Project : public LQViewable {
public:
    LI_Project(const Project* project,
               LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height);

    // void toggleListView(GLfloat y, LQMetricExpr<LQViewable>& width, LQMetricExpr<LQViewable>& height);
    void toggleListView(GLfloat y, GLfloat width, GLfloat height);
    void toggleGridView(GLfloat x, GLfloat y);

protected:
    int m_id;
};

// class LI_ProjectPreview : public LQViewable {
// public:
//     LI_ProjectPreview(const Project* project, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
// };

class UL_Project : public LQViewable {
public:
    UL_Project(LQNumber&& x, LQNumber&& y, LQNumber&& width);
    void addProject(Project* project);
    void toggleListView();
    void toggleGridView();
    void searchCallback(std::vector<Project*>& projects);
};

class DIV_Sorting : public LQViewable {
public:
    DIV_Sorting(LQNumber&& x, LQNumber&& y);
};

// class Trie : public LQViewable {
// public:
//     Trie(GLfloat x, GLfloat y);
// };

class SearchBar : public LQViewable {
public:
    SearchBar(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height);
};

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

class ProjectView : public LQViewport {
public:
    ProjectView(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
                GLint color=0xD9D9D9);  // 0x36393f
};

class DropProjectView : public LQViewport {
public:
    DropProjectView(
        LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
        GLint color=0xD9D9D9);  // 0x36393f
};
