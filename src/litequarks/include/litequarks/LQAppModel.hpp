#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "LQDeclaration.hpp"
#include "LQEvent.hpp"
#include "LQRawData.hpp"

class LQAppModel {
public:
    static void
    init();

    static std::vector<void*>&
    get(const std::string& model);

    static void*
    getFirst(const std::string& model);

    static void*
    getNth(const std::string& model, LQindex nth);

    static void
    createModel(const std::string& name, LQItemCreator creator);

    static void
    createItem(const std::string& model, LQRawData& rawData);

    static void
    dataQuery(const std::string& query);

    static void
    dataReceivedCallback(LQDataReceivedEvent& event);

protected:
    static std::unordered_map<std::string, LQItemCreator>
    s_itemCreators;

    static std::unordered_map<std::string, std::vector<void*>>
    s_items;
};
