#pragma once

#include <string>
#include <unordered_map>
#include <forward_list>

#include "LQDeclaration.hpp"
#include "LQAppController.hpp"
#include "LQRawData.hpp"

class LQAppModel {
public:
    static void
    addModelCreator(const std::string& name, LQModelCreator creator);

    static void
    createModel(char* data, LQsize size);

    static void
    dataQuery(std::vector<std::string> models);

private:
    static std::unordered_map<std::string, LQModelCreator>
    s_creators;

    static std::unordered_map<std::string, std::forward_list<void*>>
    s_models;
};

// void lqCreateModel(const char* name, LQModelCreator creator) {
//     // add model to appmodel
// }
