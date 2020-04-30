#pragma once

#include <string>

#include <litequarks/LQViewport.hpp>

class SignInView : public LQViewport {
public:
    SignInView(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
               int color=0xD9D9D9);

    LQViewable* navbar;
    LQViewable* viewport;
};

#include <litequarks/litequarks.hpp>
#include "model.hpp"

class IMG : public LQViewable {
public:
    IMG(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
        GLint color, const std::string& path);

    void draw() override;
};

class PasswordArea : public LQTextArea {
public:
    PasswordArea(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
                 GLint color, const std::string& placeholder="",
                 const std::function<void(const std::string&)>& callback = {});

    void onKey(LQKeyEvent& event) override;
    void onChar(LQCharEvent& event) override;

    std::string getContent() const override;

protected:
    std::string m_password;
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

class SearchBar : public LQViewable {
public:
    SearchBar(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height);
};

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
