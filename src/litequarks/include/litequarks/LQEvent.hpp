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

    LQRawData rawData;
};

class LQModelUpdateEvent : public LQEvent {
public:
    LQModelUpdateEvent(std::vector<std::pair<std::string, LQindex>>&& infos);

    const std::vector<std::pair<std::string, LQindex>> infos;
};
