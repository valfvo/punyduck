#include <litequarks/LQAppController.hpp>
#include <litequarks/LQAppModel.hpp>
#include <litequarks/LQRawData.hpp>
#include <iostream>
#include <typeinfo>

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
    lqOn<LQLoginEvent>(loginCallback); // Dans la class bouton login
    lqOn<LQRegisterEvent>(registerCallback); // Dans la class bouton register
    lqOn<LQUpProjectEvent>(upProjectCallback); // Dans la class bouton projet
    lqOn<LQTempActionEvent>(tempActionCallback);
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
        std::cout << "event : " << (event->type).name() << std::endl;
        auto dispatch =
            s_eventDispatcher[std::make_pair(event->type, event->target)];
        // std::cout << "dispatch vide ?" << (dispatch == nullptr) << std::endl;
        dispatch(event);
        s_eventQueue.pop();
    }
}

void LQAppController::pollResponses() {
    std::lock_guard<std::mutex> lock(s_mutex);
    while (!s_responses.empty()) {
        LQRawData data(s_responses.front());
        std::string type = data.parse<char*>();
        std::cout << "type : " << type << std::endl;
        if(type == "dataReceive") {
            LQAppController::pushEvent(new LQDataReceivedEvent(data));
        }
        if(type == "login") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Login ou password incorrect. Entrez login, password : " << std::endl;
                std::string login, password;
                std::cin >> login >> password; // >> email;
                LQAppController::pushEvent(new LQLoginEvent(login, password)); // <- Ici event pour demander un login (Vue)
            }
            else {
                std::cout << "Vous etes correctement connecte" << std::endl;
                int action;
                std::cout << "Choissisez une action (1 login 2 register 3 upProjet)" << std::endl;
                std::cin >> action;
                LQAppController::pushEvent(new LQTempActionEvent(action));
            }
        }
        if(type == "register") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Login deja pris. Entrez login, password email : " << std::endl;
                std::string login, password, email;
                std::cin >> login >> password >> email;
                LQAppController::pushEvent(new LQRegisterEvent(login, password, email)); // <- Ici event pour demander un login (Vue)
            }
            else {
                std::cout << "Vous etes correctement inscrit" << std::endl;
                int action;
                std::cout << "Choissisez une action (1 login 2 register 3 upProjet)" << std::endl;
                std::cin >> action;
                LQAppController::pushEvent(new LQTempActionEvent(action));
            }
        }
        if(type == "upProject") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Nom deja pris. Entrez nom, chemin : " << std::endl;
                std::string nom, chemin;
                std::cin >> nom;
                std::getline(std::cin, chemin);
                LQAppController::pushEvent(new LQUpProjectEvent(nom, chemin)); // <- Ici event pour demander un nom de projet (Vue)
            }
        }
        if(type == "projectUploaded") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Erreur lors de l'upload du projet. Fin de programme" << std::endl;
            }
            else {
                std::cout << "Projet mit sur le serveur." << std::endl;
                int action;
                std::cout << "Choissisez une action (1 login 2 register 3 upProjet)" << std::endl;
                std::cin >> action;
                LQAppController::pushEvent(new LQTempActionEvent(action));
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

void LQAppController::upProjectCallback(LQUpProjectEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 4;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::tempActionCallback(LQTempActionEvent& event) {
    std::string login, password, email, nom, chemin;
    switch (event.action) {
    case 1:
        std::cout << "Entrez login, password : " << std::endl;
        std::cin >> login >> password;
        LQAppController::pushEvent(new LQLoginEvent(login, password)); // Dans la class bouton register
        break;

    case 2:
        std::cout << "Entrez login, password, email : " << std::endl;
        std::cin >> login >> password >> email;
        LQAppController::pushEvent(new LQRegisterEvent(login, password, email)); // Dans la class bouton register
        break;

    case 3:
        std::cout << "Entrez nom, chemin : " << std::endl;
        std::cin >> nom;
        std::getline(std::cin, chemin);
        LQAppController::pushEvent(new LQUpProjectEvent(nom, chemin)); // Dans la class bouton register
        break;

    default:
        break;
    }
}