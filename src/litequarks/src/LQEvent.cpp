#include <litequarks/LQEvent.hpp>

LQEvent::LQEvent(const std::type_info& _type, void* _target)
: type(_type), target(_target)
{ }

LQEvent::~LQEvent() { }

LQDataQueryEvent::LQDataQueryEvent(void* _target, const std::string& _query)
: LQEvent(typeid(LQDataQueryEvent), _target), query(_query)
{ }

LQDataReceivedEvent::LQDataReceivedEvent(
    const std::string& _model, LQsize _itemCount, LQRawData _rawData)
: LQEvent(typeid(LQDataReceivedEvent), nullptr),
  model(_model), itemCount(_itemCount), rawData(_rawData)
{ }

LQModelUpdateEvent::LQModelUpdateEvent(
    std::vector<std::pair<std::string, LQindex>>&& _infos)
: LQEvent(typeid(LQModelUpdateEvent), nullptr), infos(_infos)
{ }
