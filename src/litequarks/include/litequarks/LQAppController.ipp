#include "LQAppController.hpp"

template<class TEvent>
bool LQAppController::hasCallback(void* target) {
    auto event = std::make_pair(std::type_index(typeid(TEvent)), target);
    return s_eventDispatcher.find(event) != s_eventDispatcher.end();
}

template<class TEvent>
LQViewable* LQAppController::getEligible() {
    LQViewable* eligible = s_hover_focus;
    while (!hasCallback<TEvent>(eligible)) {
        eligible = static_cast<LQViewable*>(eligible->parent());
    }
    return eligible;
}

template<class T, class TArg, void (T::*callback)(TArg*)>
void lqInvokeCallback(void* t, void* arg)
{
    (static_cast<T*>(t)->*callback)(static_cast<TArg*>(arg));
}

template<class TItem, class T, void (T::*callback)(TItem*)>
void lqForEach(T* t)
{
    LQAppController::addObserver(
        LQAppModel::getModelName(typeid(TItem)),
        t, &lqInvokeCallback<T, TItem, callback>);
}

template<class TEvent>
void lqOn(void (*callback)(TEvent&))
{
    LQAppController::addDispatcher(std::type_index(typeid(TEvent)), nullptr,
        [=](LQEvent* event) {
            callback(*static_cast<TEvent*>(event));
        });
}

template<class TEvent>
void lqOn(void (*callback)())
{
    LQAppController::addDispatcher(std::type_index(typeid(TEvent)), nullptr,
        [=](LQEvent* event) {
            callback();
        });
}

template<class TEvent, class TTarget>
void lqOn(TTarget* target, void (TTarget::*callback)(TEvent&))
{
    LQAppController::addDispatcher(std::type_index(typeid(TEvent)), target,
        [=](LQEvent* event) {
            (target->*callback)(*static_cast<TEvent*>(event));
        });
}

template<class TEvent, class TTarget>
void lqOn(TTarget* target, void (TTarget::*callback)())
{
    LQAppController::addDispatcher(std::type_index(typeid(TEvent)), target,
        [=](LQEvent* event) {
            (target->*callback)();
        });
}
