#include <litequarks/LQAppController.hpp>
#include <litequarks/LQAppModel.hpp>
#include <litequarks/LQRawData.hpp>
#include <iostream>
#include <typeinfo>

int i =24;

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
    lqOn<loginEvent>(loginCallback); // Dans la class bouton login
    lqOn<registerEvent>(registerCallback); // Dans la class bouton register
    lqOn<upProjectEvent>(upProjectCallback); // Dans la class bouton projet
    lqOn<dlProjectEvent>(dlProjectCallback); // Dans la class bouton projet
    lqOn<tempActionEvent>(tempActionCallback);
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
                LQAppController::pushEvent(new loginEvent(login, password)); // <- Ici event pour demander un login (Vue)
            }
            // else {
            //     std::cout << "Vous etes correctement connecte" << std::endl;
            //     int action;
            //     std::cout << "Choissisez une action (1 login 2 register 3 upProjet 4 dlProject)" << std::endl;
            //     std::cin >> action;
            //     LQAppController::pushEvent(new tempActionEvent(action));
            // }
        }
        if(type == "register") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Login deja pris. Entrez login, password email : " << std::endl;
                std::string login, password, email;
                std::cin >> login >> password >> email;
                LQAppController::pushEvent(new registerEvent(login, password, email)); // <- Ici event pour demander un login (Vue)
            }
            else {
                std::cout << "Vous etes correctement inscrit" << std::endl;
                int action;
                std::cout << "Choissisez une action (1 login 2 register 3 upProjet 4 dlProject)" << std::endl;
                std::cin >> action;
                LQAppController::pushEvent(new tempActionEvent(action));
            }
        }
        if(type == "upProject") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Nom deja pris. Entrez nom, chemin : " << std::endl;
                std::string nom, chemin, tag, descr, pathImage;
                std::cin >> nom;
                std::getline(std::cin, chemin);
                LQAppController::pushEvent(new upProjectEvent(chemin, nom, tag, descr, pathImage)); // Dans la class bouton register
            }
        }
        if(type == "projectUploaded") {
            int rep = data.parse<int8_t>();
            std::cout << "OUUUUI" << std::endl;
            if(rep == 0) {
                std::cout << "Erreur lors de l'upload du projet. Fin de programme" << std::endl;
            }
            else if(i < 23) {
                std::cout << "Projet uploaded" << std::endl;
                std::string nom = "Projet";
                std::string tag = "tag";
                std::string chemin =  R"(D:/Travail/Licence/Semestre 4/HLIN405 - Projet PunyDuck/src/app/test)";
                std::string descr = R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras ligula tortor, auctor sed gravida quis, finibus et sem. Donec rhoncus justo lacus, vel dictum mi consectetur eu. Cras congue nisl sit amet libero tristique, ullamcorper euismod risus scelerisque. Nullam aliquam ante ac efficitur hendrerit. Nam a tortor sed sapien placerat porta sed ullamcorper est. Aenean semper arcu ut leo porttitor, et tempus risus fringilla. Donec blandit maximus nunc, at lacinia est cursus nec. Vivamus eget eros vitae dui efficitur suscipit. Mauris blandit tempus justo et molestie.
                Cras sagittis vehicula nibh, in facilisis ipsum sagittis a. Curabitur ut aliquet orci. Nulla fermentum dolor eget lectus sagittis cursus. Nam auctor rutrum sodales. Aliquam risus tortor, facilisis imperdiet urna non, ornare ultrices velit. Vivamus mattis aliquam enim ut interdum. Duis a posuere elit, in rhoncus odio.
                
                Sed sed lorem efficitur, suscipit ante eget, laoreet massa. Suspendisse pellentesque cursus lectus, et tincidunt mauris commodo ac. Aliquam erat volutpat. Nam a ex non sapien ornare iaculis. Integer nec leo accumsan, rutrum lectus sit amet, tempus dolor. Morbi orci sapien, porttitor eu ultricies vel, eleifend sit amet dui. Fusce vitae elit a diam tristique gravida convallis commodo tortor.
                Aliquam sed nisi nisi. Aliquam tempor egestas nulla, congue vestibulum purus cursus in. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Ut nec erat vitae erat pellentesque finibus nec sed ante. Sed sed posuere leo. Quisque fermentum dolor id ex suscipit placerat eget ac dui. Nam vitae lobortis justo, ut porttitor risus. Nulla venenatis maximus turpis, et ullamcorper quam dapibus sed. Praesent id fringilla metus, eu elementum odio. Nullam urna libero, tincidunt nec nulla nec, imperdiet sollicitudin nunc. Phasellus ut fermentum lectus, a porttitor nisl. Mauris eu sagittis leo. Pellentesque non enim vulputate, posuere risus ac, mattis ante. Aliquam erat volutpat.
                Vivamus rhoncus ac elit ut pellentesque. Etiam aliquet lorem risus, id porta metus pharetra a. Cras volutpat elit non sapien facilisis, eu volutpat augue sollicitudin. Vivamus pretium augue massa, ac elementum leo condimentum at. Vivamus vitae odio et sem ultricies convallis nec ut est. Maecenas eleifend et velit sit amet convallis. Fusce ligula turpis, pretium id pellentesque vel, cursus vel ligula. Nulla rhoncus nibh eget dui fringilla porta. Cras aliquet, mi ac varius cursus, tellus elit condimentum magna, vitae congue nulla purus et orci. Pellentesque ullamcorper, sem sed ornare blandit, lacus risus porta nunc, non consectetur eros lacus sed tortor. Duis euismod elementum maximus. In at blandit orci. Etiam mollis, nisi sit amet faucibus semper, augue eros gravida metus, non consequat est odio vitae ipsum. Nunc eu ligula vel arcu laoreet ultrices. Cras a dolor eros.
                
                Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras ligula tortor, auctor sed gravida quis, finibus et sem. Donec rhoncus justo lacus, vel dictum mi consectetur eu. Cras congue nisl sit amet libero tristique, ullamcorper euismod risus scelerisque. Nullam aliquam ante ac efficitur hendrerit. Nam a tortor sed sapien placerat porta sed ullamcorper est. Aenean semper arcu ut leo porttitor, et tempus risus fringilla. Donec blandit maximus nunc, at lacinia est cursus nec. Vivamus eget eros vitae dui efficitur suscipit. Mauris blandit tempus justo et molestie.
                Cras sagittis vehicula nibh, in facilisis ipsum sagittis a. Curabitur ut aliquet orci. Nulla fermentum dolor eget lectus sagittis cursus. Nam auctor rutrum sodales. Aliquam risus tortor, facilisis imperdiet urna non, ornare ultrices velit. Vivamus mattis aliquam enim ut interdum. Duis a posuere elit, in rhoncus odio.
                
                Sed sed lorem efficitur, suscipit ante eget, laoreet massa. Suspendisse pellentesque cursus lectus, et tincidunt mauris commodo ac. Aliquam erat volutpat. Nam a ex non sapien ornare iaculis. Integer nec leo accumsan, rutrum lectus sit amet, tempus dolor. Morbi orci sapien, porttitor eu ultricies vel, eleifend sit amet dui. Fusce vitae elit a diam tristique gravida convallis commodo tortor.
                Aliquam sed nisi nisi. Aliquam tempor egestas nulla, congue vestibulum purus cursus in. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Ut nec erat vitae erat pellentesque finibus nec sed ante. Sed sed posuere leo. Quisque fermentum dolor id ex suscipit placerat eget ac dui. Nam vitae lobortis justo, ut porttitor risus. Nulla venenatis maximus turpis, et ullamcorper quam dapibus sed. Praesent id fringilla metus, eu elementum odio. Nullam urna libero, tincidunt nec nulla nec, imperdiet sollicitudin nunc. Phasellus ut fermentum lectus, a porttitor nisl. Mauris eu sagittis leo. Pellentesque non enim vulputate, posuere risus ac, mattis ante. Aliquam erat volutpat.
                Vivamus rhoncus ac elit ut pellentesque. Etiam aliquet lorem risus, id porta metus pharetra a. Cras volutpat elit non sapien facilisis, eu volutpat augue sollicitudin. Vivamus pretium augue massa, ac elementum leo condimentum at. Vivamus vitae odio et sem ultricies convallis nec ut est. Maecenas eleifend et velit sit amet convallis. Fusce ligula turpis, pretium id pellentesque vel, cursus vel ligula. Nulla rhoncus nibh eget dui fringilla porta. Cras aliquet, mi ac varius cursus, tellus elit condimentum magna, vitae congue nulla purus et orci. Pellentesque ullamcorper, sem sed ornare blandit, lacus risus porta nunc, non consectetur eros lacus sed tortor. Duis euismod elementum maximus. In at blandit orci. Etiam mollis, nisi sit amet faucibus semper, augue eros gravida metus, non consequat est odio vitae ipsum. Nunc eu ligula vel arcu laoreet ultrices. Cras a dolor eros.)";
                i++;
                nom += std::to_string(i);
                tag += std::to_string(i);
                LQAppController::pushEvent(new upProjectEvent(chemin, nom, tag, descr, "D:/Images/pfp.PNG")); // Dans la class bouton register
            }
        }
        if(type == "projectDownloaded") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                std::cout << "Erreur lors du telechargement du projet. Fin de programme" << std::endl;
            }
            else {
                std::cout << "Projet telecharge." << std::endl;
                int action;
                std::cout << "Choissisez une action (1 login 2 register 3 upProjet 4 dlProject)" << std::endl;
                std::cin >> action;
                LQAppController::pushEvent(new tempActionEvent(action));
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

void LQAppController::loginCallback(loginEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 2;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::registerCallback(registerEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 3;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::upProjectCallback(upProjectEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 4;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::dlProjectCallback(dlProjectEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[5];
    query[0] = 5;
    memcpy(&query[1], &event.idProject, 4);
    s_queries.push(query);
}

void LQAppController::tempActionCallback(tempActionEvent& event) {
    std::string login, password, email, nom, chemin, tag, descr, pathImage;
    switch (event.action) {
    case 1:
        std::cout << "Entrez login, password : " << std::endl;
        std::cin >> login >> password;
        LQAppController::pushEvent(new loginEvent(login, password)); // Dans la class bouton login
        break;

    case 2:
        std::cout << "Entrez login, password, email : " << std::endl;
        std::cin >> login >> password >> email;
        LQAppController::pushEvent(new registerEvent(login, password, email)); // Dans la class bouton projet
        break;

    case 3:
        std::cout << "Entrez chemin, nom, tag, pathImage : " << std::endl;
        std::getline(std::cin, chemin);
        std::getline(std::cin, nom);
        std::getline(std::cin, tag);
        descr = R"("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras ligula tortor, auctor sed gravida quis, finibus et sem. Donec rhoncus justo lacus, vel dictum mi consectetur eu. Cras congue nisl sit amet libero tristique, ullamcorper euismod risus scelerisque. Nullam aliquam ante ac efficitur hendrerit. Nam a tortor sed sapien placerat porta sed ullamcorper est. Aenean semper arcu ut leo porttitor, et tempus risus fringilla. Donec blandit maximus nunc, at lacinia est cursus nec. Vivamus eget eros vitae dui efficitur suscipit. Mauris blandit tempus justo et molestie.
Cras sagittis vehicula nibh, in facilisis ipsum sagittis a. Curabitur ut aliquet orci. Nulla fermentum dolor eget lectus sagittis cursus. Nam auctor rutrum sodales. Aliquam risus tortor, facilisis imperdiet urna non, ornare ultrices velit. Vivamus mattis aliquam enim ut interdum. Duis a posuere elit, in rhoncus odio.
Sed sed lorem efficitur, suscipit ante eget, laoreet massa. Suspendisse pellentesque cursus lectus, et tincidunt mauris commodo ac. Aliquam erat volutpat. Nam a ex non sapien ornare iaculis. Integer nec leo accumsan, rutrum lectus sit amet, tempus dolor. Morbi orci sapien, porttitor eu ultricies vel, eleifend sit amet dui. Fusce vitae elit a diam tristique gravida convallis commodo tortor.
Aliquam sed nisi nisi. Aliquam tempor egestas nulla, congue vestibulum purus cursus in. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Ut nec erat vitae erat pellentesque finibus nec sed ante. Sed sed posuere leo. Quisque fermentum dolor id ex suscipit placerat eget ac dui. Nam vitae lobortis justo, ut porttitor risus. Nulla venenatis maximus turpis, et ullamcorper quam dapibus sed. Praesent id fringilla metus, eu elementum odio. Nullam urna libero, tincidunt nec nulla nec, imperdiet sollicitudin nunc. Phasellus ut fermentum lectus, a porttitor nisl. Mauris eu sagittis leo. Pellentesque non enim vulputate, posuere risus ac, mattis ante. Aliquam erat volutpat.
Vivamus rhoncus ac elit ut pellentesque. Etiam aliquet lorem risus, id porta metus pharetra a. Cras volutpat elit non sapien facilisis, eu volutpat augue sollicitudin. Vivamus pretium augue massa, ac elementum leo condimentum at. Vivamus vitae odio et sem ultricies convallis nec ut est. Maecenas eleifend et velit sit amet convallis. Fusce ligula turpis, pretium id pellentesque vel, cursus vel ligula. Nulla rhoncus nibh eget dui fringilla porta. Cras aliquet, mi ac varius cursus, tellus elit condimentum magna, vitae congue nulla purus et orci. Pellentesque ullamcorper, sem sed ornare blandit, lacus risus porta nunc, non consectetur eros lacus sed tortor. Duis euismod elementum maximus. In at blandit orci. Etiam mollis, nisi sit amet faucibus semper, augue eros gravida metus, non consequat est odio vitae ipsum. Nunc eu ligula vel arcu laoreet ultrices. Cras a dolor eros.")";
        std::getline(std::cin, pathImage);
        LQAppController::pushEvent(new upProjectEvent(chemin, nom, tag, descr, pathImage)); // Dans la class bouton register
        break;
    
    case 4:
        LQAppController::pushEvent(new dlProjectEvent(29)); // Dans la class bouton projet


    default:
        break;
    }
}