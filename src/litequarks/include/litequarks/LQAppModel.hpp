#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <typeinfo>

#include "LQDeclaration.hpp"
#include "LQEvent.hpp"
#include "LQRawData.hpp"

class LQAppModel {
public:
    static void
    init();

    static std::vector<LQitem*>&
    get(const std::string& model);

    static LQitem*
    getFirst(const std::string& model);

    static LQitem*
    getNth(const std::string& model, LQindex nth);

    static const std::string&
    getModelName(const std::type_info& type);

    static void
    createModel(const std::string& name, const std::type_info& type,
                LQItemCreator creator);

    static void
    createItem(const std::string& model, LQRawData& rawData,
               const bool* attributes);

    static void
    dataQuery(const std::string& query);

    static void
    dataReceivedCallback(LQDataReceivedEvent& event);

protected:
    static std::unordered_map<std::string, LQItemCreator>
    s_itemCreators;

    static std::unordered_map<std::string, std::vector<LQitem*>>
    s_items;

    static std::unordered_map<std::type_index, std::string>
    s_modelNames;
};