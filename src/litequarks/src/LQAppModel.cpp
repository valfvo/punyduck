#include <utility>  // std::move
#include <typeindex>

#include <litequarks/LQAppController.hpp>
#include <litequarks/LQAppModel.hpp>

std::unordered_map<std::string, LQItemCreator>
LQAppModel::s_itemCreators;

std::unordered_map<std::string, std::vector<void*>>
LQAppModel::s_items;

std::unordered_map<std::type_index, std::string>
LQAppModel::s_modelNames;

void LQAppModel::init() {
    lqOn<LQDataReceivedEvent>(dataReceivedCallback);
}

std::vector<void*>& LQAppModel::get(const std::string& model) {
    return s_items[model];
}

void* LQAppModel::getFirst(const std::string& model) {
    return getNth(model, 0);
}

void* LQAppModel::getNth(const std::string& model, LQindex nth) {
    auto& items = s_items[model];
    LQsize size = items.size();
    if (nth < size) {
        return items[nth];
    }
    else {
        return nullptr;
    }
}

const std::string& LQAppModel::getModelName(const std::type_info& type) {
    return s_modelNames[std::type_index(type)];
}

void LQAppModel::createModel(
    const std::string& name, const std::type_info& type,
    LQItemCreator creator)
{
    s_modelNames.insert({std::type_index(type), name});
    s_itemCreators.insert({name, creator});
}

void LQAppModel::createItem(const std::string& model, LQRawData& rawData,
                            const bool* attributes)
{
    s_items[model].push_back(s_itemCreators[model](rawData, attributes));
}

void LQAppModel::dataQuery(const std::string& query) {
    LQAppController::pushEvent(new LQDataQueryEvent(nullptr, query));
}

void LQAppModel::dataReceivedCallback(LQDataReceivedEvent& event) {
    std::vector<std::pair<std::string, LQindex>> infos;
    infos.push_back({event.model, get(event.model).size()});

    for (LQindex i = 0; i < event.itemCount; ++i) {
        LQAppModel::createItem(event.model, event.rawData, event.attributes);
    }

    LQAppController::pushEvent(new LQModelUpdateEvent(std::move(infos)));
}