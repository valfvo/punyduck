#include <utility>  // std::move

#include <litequarks/LQAppController.hpp>
#include <litequarks/LQAppModel.hpp>

std::unordered_map<std::string, LQItemCreator>
LQAppModel::s_itemCreators;

std::unordered_map<std::string, std::vector<void*>>
LQAppModel::s_items;

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

void LQAppModel::createModel(const std::string& name, LQItemCreator creator) {
    s_itemCreators.insert({name, creator});
}

void LQAppModel::createItem(const std::string& model, LQRawData& rawData) {
    s_items[model].push_back(s_itemCreators[model](rawData));
}

void LQAppModel::dataQuery(std::string models) {
    // LQDataQueryEvent event("dataQuery", "SELECT * FROM ");
    // event.query += model + ';';

    // AppController::s_eventQueue.push_back(event);
}

void LQAppModel::dataReceivedCallback(LQDataReceivedEvent& event) {
    std::vector<std::pair<std::string, LQindex>> infos;
    infos.push_back({event.model, get(event.model).size()});

    for (LQindex i = 0; i < event.itemCount; ++i) {
        LQAppModel::createItem(event.model, event.rawData);
    }

    LQAppController::pushEvent(new LQModelUpdateEvent(std::move(infos)));
}
