#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "LQAppModel.hpp"
#include "LQEvent.hpp"

class LQAppController {
public:
    LQAppController() {
        // listen('modelupdate', this);
    }

    static void pollEvents() {
        while (!LQAppController::s_eventQueue.empty()) {
            // auto event = LQAppController::s_eventQueue.pop();
            // for (callback : callbacks[event.name]) {
            //     viewable->callback(event);
            // }
        }
    }

    static void modelUpdateCallback(LQModelUpdateEvent& event) {
        for (auto& info : event.infos) {
            auto& items = LQAppModel::s_items[info.first];
            auto& observers = LQAppController::s_modelObservers[info.first];
            auto it = items.cbegin() + info.second;

            for (auto pair : observers) {
                for (auto item = it; item != items.cend(); ++item) {
                    (pair.first)(pair.second, *item);
                }
            }
        }
    }

    static std::queue<LQEvent> s_eventQueue;
    static std::unordered_map<const char*,
        std::vector<std::pair<std::function<void(void*, void*)>, void*>>>
    s_modelObservers;
    // [model]{{void invoke(void* observer, void* item), void* observer}, ...}
};

std::unordered_map<const char*,
    std::vector<std::pair<std::function<void(void*, void*)>, void*>>>
LQAppController::s_modelObservers;

template<class T, class TArg, void (T::*callback)(TArg*)>
void lqInvokeCallback(void* t, void* arg) {
    (static_cast<T*>(t)->*callback)(static_cast<TArg*>(arg));
}

template<class TItem, class T, void (T::*callback)(TItem*)>
void lqForEach(T* t) {
    LQAppController::s_modelObservers[typeid(TItem).name()]
        .push_back(std::make_pair(&lqInvokeCallback<T, TItem, callback>, t));
}

#define LQ_FOR_EACH(item, callback)\
    lqForEach<item, std::remove_reference_t<decltype(*this)>, callback>(this)
