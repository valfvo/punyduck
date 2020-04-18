#pragma once

#include <functional>  // std::function
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utility>  // std::pair
#include <vector>
#include <cstdint>

#include "LQEvent.hpp"
#include "LQHash.hpp"
#include "LQAppModel.hpp"

#include <client/ClientGateway.hpp>

// struct LQBinData {
//     int   size;
//     char* data;
// };

// struct Project {
//     const std::string name;
//     const std::string tag;
//     const std::string desc;
//     LQBinData img;
// };

class LQAppController {
public:
    static void init();

    static void finalize();

    static void pushEvent(LQEvent* event);

    static void pushQuery(char* query);

    static void addDispatcher(std::type_index&& eventType, void* target,
        std::function<void(LQEvent*)> dispatcher);

    static void addObserver(const std::string& model, void* observer,
        void (*callback)(void*, void*));

    static void pollEvents();

    static void pollResponses();

    static void dataQueryCallback(LQDataQueryEvent& event);

    static void modelUpdateCallback(LQModelUpdateEvent& event);

    static void loginCallback(LQLoginEvent& event);

    static void registerCallback(LQRegisterEvent& event);

protected:
    static std::queue<LQEvent*>
    s_eventQueue;

    static std::unordered_map<std::pair<std::type_index, const void*>,
        std::function<void(LQEvent*)>>
    s_eventDispatcher;
    // [{event type, target}] -> lambda (which call the callback)

    static std::unordered_map<std::string,
        std::vector<std::pair<std::function<void(void*, LQitem*)>, void*>>>
    s_modelObservers;
    // [model] -> {{invoke(void* observer, void* item), void* observer}, ...}

    static std::mutex
    s_mutex;

    static std::queue<char*>
    s_queries;

    static std::queue<char*>
    s_responses;

    static ClientGateway*
    s_gateway;

    static std::thread*
    s_gatewayThread;
};

template<class T, class TArg, void (T::*callback)(TArg*)>
void lqInvokeCallback(void* t, void* arg);

template<class TItem, class T, void (T::*callback)(TItem*)>
void lqForEach(T* t);

#define LQ_FOR_EACH(item, callback)\
    lqForEach<item, std::remove_reference_t<decltype(*this)>, callback>(this)

template<class TEvent>
void lqOn(void (*callback)(TEvent&));

template<class TEvent>
void lqOn(void (*callback)());

template<class TEvent, class TTarget>
void lqOn(TTarget* target, void (TTarget::*callback)(TEvent&));

template<class TEvent, class TTarget>
void lqOn(TTarget* target, void (TTarget::*callback)());

#include "LQAppController.ipp"
