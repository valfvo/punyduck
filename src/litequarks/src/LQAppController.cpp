#include <litequarks/LQAppController.hpp>
#include <litequarks/LQAppModel.hpp>
#include <litequarks/LQRawData.hpp>
#include <iostream>

std::queue<LQEvent*>
LQAppController::s_eventQueue;

std::unordered_map<std::pair<std::type_index, const void*>,
    std::function<void(LQEvent*)>>
LQAppController::s_eventDispatcher;

std::unordered_map<std::string,
    std::vector<std::pair<std::function<void(void*, void*)>, void*>>>
LQAppController::s_modelObservers;

std::mutex
LQAppController::s_mutex;

std::queue<char*>
LQAppController::s_queries;

std::queue<char*>
LQAppController::s_responses;

ClientGateway*
LQAppController::s_gateway = nullptr;

std::thread*
LQAppController::s_gatewayThread = nullptr;

void LQAppController::init() {
    lqOn<LQDataQueryEvent>(dataQueryCallback);
    lqOn<LQModelUpdateEvent>(modelUpdateCallback);
    s_gateway = new ClientGateway(s_queries, s_responses);
    s_gatewayThread = new std::thread(std::ref(*s_gateway));
}

void LQAppController::finalize() {
    // TODO : undo lqON (here)
    s_gatewayThread->join();
    delete s_gatewayThread;
    delete s_gateway;
}

void LQAppController::pushEvent(LQEvent* event) {
    s_eventQueue.push(event);
}

void LQAppController::pushQuery(char* query) {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_queries.push(query);
}

void LQAppController::addDispatcher(
    std::type_index&& eventType, void* target,
    std::function<void(LQEvent*)> dispatcher)
{
    s_eventDispatcher[std::make_pair(eventType, target)] = dispatcher;
}

void LQAppController::addObserver(
    const std::string& model, void* observer, void (*callback)(void*, void*))
{
    s_modelObservers[model].push_back(std::make_pair(callback, observer));
}

void LQAppController::pollEvents() {
    while (!s_eventQueue.empty()) {
        auto event = s_eventQueue.front();
        auto dispatch =
            s_eventDispatcher[std::make_pair(event->type, event->target)];
        dispatch(event);
        s_eventQueue.pop();
    }
}

void LQAppController::pollResponses() {
    std::lock_guard<std::mutex> lock(s_mutex);
    while (!s_responses.empty()) {
        LQRawData data(s_responses.front());
        std::string type = data.parse<char*>();
        if(type == "dataReceive") {
            LQAppController::pushEvent(new LQDataReceivedEvent(data));
        }
        if(type == "login") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Login ou Password. Entrez login, password : " << std::endl;
                std::string login, password, email;
                std::cin >> login >> password; // >> email;
                LQAppController::pushEvent(new LQRegisterEvent(login, password, email)); // <- Ici event pour demander un login (Vue)
            }
            else {
                std::cout << "Vous etes correctement connecte" << std::endl;
            }
        }
        s_responses.pop();
    }
}

void LQAppController::dataQueryCallback(LQDataQueryEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.query.length() + 2];
    query[0] = 1;
    strcpy(&query[1], event.query.c_str());
    s_queries.push(query);
}

void LQAppController::modelUpdateCallback(LQModelUpdateEvent& event) {
    for (auto& info : event.infos) {
        auto items = LQAppModel::get(info.first);
        auto& observers = s_modelObservers[info.first];
        auto it = items.cbegin() + info.second;

        for (auto& pair : observers) {
            for (auto item = it; item != items.cend(); ++item) {
                (pair.first)(pair.second, *item);
            }
        }
    }
}

void LQAppController::loginCallback(LQLoginEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 2;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::registerCallback(LQRegisterEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 3;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}