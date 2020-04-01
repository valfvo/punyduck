#include <litequarks/LQAppModel.hpp>

std::unordered_map<std::string, LQModelCreator>
LQAppModel::s_creators;

std::unordered_map<const char*, std::vector<void*>>
LQAppModel::s_items;

void LQAppModel::addModel(
    const std::string& name, LQModelCreator creator)
{
    s_creators.insert({name, creator});
}

void LQAppModel::createItem(std::string model, LQRawData& rawData) {
    s_items[model.c_str()].push_back(s_creators[model](rawData));
}

void LQAppModel::dataQuery(std::string models) {
    // LQDataQueryEvent event("dataQuery", "SELECT * FROM ");
    // event.query += model + ';';

    // AppController::s_eventQueue.push_back(event);
}