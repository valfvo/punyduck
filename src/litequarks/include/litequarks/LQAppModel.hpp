#pragma once

#include <functional>  // std::function
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

    template<class TItem, class TTarget,
             void (TTarget::*callback)(std::vector<TItem*>&)>
    static void
    itemQuery(const std::string& model, TTarget* target,
              const std::function<bool(TItem*)>& comparator = {});

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

template<class TItem, class TTarget,
         void (TTarget::*callback)(std::vector<TItem*>&)>
void LQAppModel::itemQuery(
    const std::string& model, TTarget* target,
    const std::function<bool(TItem*)>& comparator)
{
    auto it = s_items.find(model);
    if (it != s_items.end()) {
        std::vector<TItem*> items;
        if (comparator) {
            for (auto item : it->second) {
                auto casted_item = static_cast<TItem*>(item);
                if (comparator(casted_item)){
                    items.push_back(casted_item);
                }
            }
        }
        else {
            for (auto item : it->second) {
                items.push_back(static_cast<TItem*>(item));
            }
        }
        (target->*callback)(items);
    }
}
