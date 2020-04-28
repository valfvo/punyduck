#pragma once

#include <string>
#include <typeindex>
#include <vector>

#include "LQDeclaration.hpp"
#include "LQRawData.hpp"

class LQEvent {
public:
    virtual ~LQEvent();

    const std::type_index type;
    const void* target;

protected:
    LQEvent(const std::type_info& type, void* target);
};

class LQClickEvent : public LQEvent {
public:
    LQClickEvent(void* target, float mx, float my);

    const float mx;
    const float my;
};

class LQFocusGainEvent : public LQEvent {
public:
    LQFocusGainEvent(void* target);
};

class LQFocusLoseEvent : public LQEvent {
public:
    LQFocusLoseEvent(void* target);
};

class LQCharEvent : public LQEvent {
public:
    LQCharEvent(void* target, char32_t codepoint);

    const std::u32string codepoint;
};

class LQKeyEvent : public LQEvent {
public:
    LQKeyEvent(void* target, int key, int action, int mods);

    const int key;
    const int action;
    const int mods;
};

class LQDataQueryEvent : public LQEvent {
public:
    LQDataQueryEvent(void* target, const std::string& query);

    const std::string query;
};

class LQDataReceivedEvent : public LQEvent {
public:
    LQDataReceivedEvent(LQRawData& data);
    ~LQDataReceivedEvent() override;

    const std::string model;
    const LQsize itemCount;
    const bool* attributes;

    LQRawData data;
};

class LQModelUpdateEvent : public LQEvent {
public:
    LQModelUpdateEvent(std::vector<std::pair<std::string, LQindex>>&& infos);

    const std::vector<std::pair<std::string, LQindex>> infos;
};

class loginEvent : public LQEvent {
public:
    loginEvent(std::string login, std::string password);

    const std::string infos;
};

class registerEvent : public LQEvent {
public:
    registerEvent(std::string login, std::string password, std::string email);

    const std::string infos;
};

class upProjectEvent : public LQEvent {
public:
    upProjectEvent(std::string chemin, std::string nom, std::string tag, std::string descr, std::string pathImage);

    const std::string infos;
};

class dlProjectEvent : public LQEvent {
public:
    dlProjectEvent(int _idProject);

    const int idProject;
};

class tempActionEvent : public LQEvent {
public:
    tempActionEvent(int _action);

    const int action;
};