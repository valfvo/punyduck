#include <litequarks/LQEvent.hpp>
#include <cstdint>
#include <iostream>
LQEvent::LQEvent(const std::type_info& _type, void* _target)
: type(_type), target(_target)
{ }

LQEvent::~LQEvent() { }

LQDataQueryEvent::LQDataQueryEvent(void* _target, const std::string& _query)
: LQEvent(typeid(LQDataQueryEvent), _target), query(_query)
{ }

#include <iostream>
LQDataReceivedEvent::LQDataReceivedEvent(LQRawData& _data)
: LQEvent(typeid(LQDataReceivedEvent), nullptr),
  model(_data.parse<char*>()), itemCount(_data.parse<int32_t>()),
  attributes(nullptr), data(_data)
{
    int attrCount = data.parse<int8_t>();
    std::cout << "attrCount" << attrCount << std::endl;
    bool* attr = new bool[attrCount];
    for (int i = 0; i < attrCount; ++i) {
        attr[i] = false;
    }
    int attrReceivedCount = data.parse<int32_t>();
    for (int i = 0; i < attrReceivedCount; ++i) {
        attr[data.parse<int32_t>()] = true;
    }
    attributes = attr;
}

LQDataReceivedEvent::~LQDataReceivedEvent() {
    delete[] attributes;
};

LQModelUpdateEvent::LQModelUpdateEvent(
    std::vector<std::pair<std::string, LQindex>>&& _infos)
: LQEvent(typeid(LQModelUpdateEvent), nullptr), infos(_infos)
{ }

loginEvent::loginEvent(std::string login, std::string password)
: LQEvent(typeid(loginEvent), nullptr), infos(login+'|'+password)
{ }

registerEvent::registerEvent(std::string login, std::string password, std::string email)
: LQEvent(typeid(registerEvent), nullptr), infos(login+'|'+password+'|'+email)
{ }

upProjectEvent::upProjectEvent(std::string chemin, std::string nom, std::string tag, std::string descr, std::string pathImage)
: LQEvent(typeid(upProjectEvent), nullptr), infos(chemin+'|'+nom+'|'+tag+'|'+descr+'|'+pathImage)
{ }

dlProjectEvent::dlProjectEvent(int _idProject)
: LQEvent(typeid(dlProjectEvent), nullptr), idProject(_idProject)
{ }

tempActionEvent::tempActionEvent(int _action)
: LQEvent(typeid(tempActionEvent), nullptr), action(_action)
{ }