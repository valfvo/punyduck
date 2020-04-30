#pragma once

#include <functional>  // std::function
#include <mutex>
#include <queue>
#include <thread>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <utility>  // std::pair
#include <vector>
#include <string>
#include <cstdint>

#include "LQWindow.hpp"
#include "LQEvent.hpp"
#include "LQHash.hpp"
#include "LQAppModel.hpp"
#include <GLFW/glfw3.h>

#include <client/ClientGateway.hpp>

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

    template<class TEvent>
    static bool hasCallback(void* target);

    template<class TEvent>
    static LQViewable* getEligible();

    static void setWindow(LQWindow* window);

    static void removeFocus(LQViewable* viewable);

    static void resetMousePosition();

    static void updateMousePosition();

    static void recalcMousePosition(float xoffset, float yoffset);

    static void cursor_position_callback(GLFWwindow* window, double mx, double my);

    static void mouse_button_callback(GLFWwindow* window, int button,
                                      int action, int mods);

    static void key_callback(GLFWwindow* window, int key, int scancode,
                             int action, int mods);

    static void character_callback(GLFWwindow* window, unsigned int codepoint);
  
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static void drop_callback(GLFWwindow* window, int count, const char** paths);

    static void dataQueryCallback(LQDataQueryEvent& event);

    static void modelUpdateCallback(LQModelUpdateEvent& event);

    static void loginCallback(loginEvent& event);

    static void registerCallback(registerEvent& event);

    static void upProjectCallback(upProjectEvent& event);

    static void dlProjectCallback(dlProjectEvent& event);

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

    static LQWindow*
    s_window;

    static LQViewable*
    s_hover_focus;

    static LQViewable*
    s_focus;

    static float prevAbsX, prevAbsY, prevRelX, prevRelY;
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
