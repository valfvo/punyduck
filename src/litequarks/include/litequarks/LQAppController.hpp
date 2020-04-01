#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "LQAppModel.hpp"
#include "LQEvent.hpp"

// button list mode
// lqOn("click", button, UL_Project*, displayList, false)
// lqOn("click", button2, UL_Project*, displayMosaique, false)

// viewable: ["event"] -> std::vector* {invoke, observer}
// event.observers -------------^

// [{event, target}]
//     {{invoke(void* observer, void* event), void* observer}, ...}

enum LQEventType {
    click,
    modelUpdate
};

"click" -> LQMouseClickEvent
"model-update" -> LQModelUpdateEvent
lqOn<LQMouseClickEvent, UL_Project, UL_Project::addProject>(&button, &ul);

template<class TEvent, class TObserver, void (TObserver::*callback)(TEvent&)>
void lqOn(std::string nom, void* target, TObserver* observer, bool capture)
{
    LQAppController::s_eventDispatcher[std::make_pair(nom, target)]
        .push_back(std::make_pair(&lqInvokeCallback<TObserver, TEvent, callback>, observer));
}

lqOn<constexpr

class LQAppController {
public:
    LQAppController() {
        // lqOn("model-update", modelUpdateCallback);
    }

    std::unordered_map<std::pair<std::string, void*>,
        std::vector<std::pair<std::function<void(void*, void*)>, void*>>>
    s_eventDispatcher;
    // [{event, target}]
    //     {{invoke(void* observer, void* event), void* observer}, ...}

    static void pollEvents() {
        // lqOn(string nom, target, observer, callback, bool capture)
        //
        while (!LQAppController::s_eventQueue.empty()) {
            auto event = LQAppController::s_eventQueue.front();
            for (auto& pair : event.observers) {
                (pair.first)(pair.second, *event);
            }
            LQAppController::s_eventQueue.pop();
        }
    }

    static void modelUpdateCallback(LQModelUpdateEvent& event) {
        for (auto& info : event.infos) {
            auto& items = LQAppModel::s_items[info.first];
            auto& observers = LQAppController::s_modelObservers[info.first];
            auto it = items.cbegin() + info.second;

            for (auto& pair : observers) {
                for (auto item = it; item != items.cend(); ++item) {
                    (pair.first)(pair.second, *item);
                }
            }
        }
    }

    static std::queue<LQEvent*> s_eventQueue;
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
