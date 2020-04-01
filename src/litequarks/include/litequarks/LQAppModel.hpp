#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "LQDeclaration.hpp"
#include "LQRawData.hpp"

class LQAppModel {
public:
    static void
    addModel(const std::string& name, LQModelCreator creator);

    static void
    createItem(std::string model, LQRawData& rawData);

    static void
    dataQuery(std::string query);

    static std::unordered_map<const char*, std::vector<void*>>
    s_items;

private:
    static std::unordered_map<std::string, LQModelCreator>
    s_creators;
};

// void lqCreateModel(const char* name, LQModelCreator creator) {
//     // add model to appmodel
// }
