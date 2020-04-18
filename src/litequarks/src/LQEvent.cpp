#include <litequarks/LQEvent.hpp>
#include <cstdint>

LQEvent::LQEvent(const std::type_info& _type, void* _target)
: type(_type), target(_target)
{ }

LQEvent::~LQEvent() { }

LQDataQueryEvent::LQDataQueryEvent(void* _target, const std::string& _query)
: LQEvent(typeid(LQDataQueryEvent), _target), query(_query)
{ }

LQDataReceivedEvent::LQDataReceivedEvent(LQRawData& data)
: LQEvent(typeid(LQDataReceivedEvent), nullptr),
  model(data.parse<char*>()), itemCount(data.parse<int32_t>()),
  attributes(nullptr), rawData(data)
{
    int attrCount = data.parse<int8_t>();
    bool* attr = new bool[attrCount];
    for (int i = 0; i < attrCount; ++i) {
        attr[i] = false;
    }
    int indiceCount = data.parse<int32_t>();
    for (int i = 0; i < indiceCount; ++i) {
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

LQLoginEvent::LQLoginEvent(std::string login, std::string password)
: LQEvent(typeid(LQLoginEvent), nullptr), infos(login+'|'+password)
{ }
LQRegisterEvent::LQRegisterEvent(std::string login, std::string password, std::string email)
: LQEvent(typeid(LQRegisterEvent), nullptr), infos(login+'|'+password+'|'+email)
{ }