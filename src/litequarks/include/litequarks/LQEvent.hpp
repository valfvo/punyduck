#pragma once

#include <utility>
#include <vector>

#include "LQDeclaration.hpp"

class LQEvent {
public:
    virtual ~LQEvent();
};

struct LQModelUpdateEvent {
    const std::vector<std::pair<const char*, LQindex>> infos;
};

struct LQDataReceiveEvent {
    std::string nom;
    LQRawData rawData;
    int size;
    std::string model;
    int nItem;
};

struct LQDataQueryEvent {
    std::string nom;
    std::string query;
};