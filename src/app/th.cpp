#include "litequarks.hpp"
#include "include/litequarks/LQRawData.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <typeinfo>
#include <utility>

std::mutex app_mutex;
std::vector<char*> requests;
std::vector<char*> responses;

char* pollResponses(std::vector<char*>& responses) {
    std::lock_guard<std::mutex> lock(app_mutex);
    if (!responses.empty()) {
        std::cout << "Response of py = " << responses[0] << std::endl;
        char* save = responses[0];
        responses.erase(responses.begin());
        int size = (int*)save;
        LQRawData rawData(save, size);
        std::string head(rawData.parse<char*>());
        if(head == "response") {
            head.assign(rawData.parse<char*>());
            int nItem = rawData.parse<int>();
            LQDataReceiveEvent event("dataReceive", rawData, size, head, nItem);
            LQAppController::s_eventQueue.push_back(event);
        }
        return save;
    }
    return nullptr;
}

void LQDataQueryEventCB(LQDataQueryEvent event) {
    event.query = 1 + event.query
    requests.push_back(event.query);
}

void LQDataReceiveEventCB(LQDataReceiveEvent event) {
    std::vector<std::pair<const char*, LQindex>> infos;
    infos.push_back(std::make_pair(event.model, LQAppModel::s_items[event.model].size()));

    for(int i = 0; i < event.nItems; i++) {
        LQAppModel::createItem(event.model, event.rawData);
    }

    LQModelUpdateEvent eventMU(infos);
    LQAppController::s_eventQueue.push_back(eventMU);
}

struct Img {
    char* bin;
    int size;
};

struct Projet {
    std::string title;
    std::string descr;
    Img image;
    std::string date;
};

std::vector<(*lambda)(reader)> array;
std::vector<std::string> index_array;

void createModel(std::string name, Projet (*lambda)(reader)) {
    array.push_back(lambda);
    index_array.push_back(name);
}

std::string Qregister() {
    std::string login, password, email, query;
    std::cout << "Entrez un login : ";
    std::cin >> login;
    std::cout << "Entrez un password :";
    std::cin >> password;
    while(password.length() < 8) {
        std::cout << "Mot de passe trop court, veuillez en choisir un autre : ";
        std::cin >> password;
    }
    std::cout << "Entrez un email :";
    std::cin >> email;

    query = '1'+login+'|'+password+'|'+email;
    std::cout << "Query = " << query << std::endl;

    return query;
}

int main(int argc, char* argv[])
{
    ClientGateway gateway(requests, responses);
    std::thread gatewayThread(std::ref(gateway));

    std::string query;
    char* c_query;
    bool connected = true;
    char* rep;
    while(connected) {
        std::cout << "Requête : ";
        std::cin >> query;


        if(query == "exit")
        {
            std::cout << "Exit..." << std::endl;
            char* c = (char*)"0";//itoa(0, c, 10);
            requests.push_back(c);
            connected = false;
        }

        if(query == "register") { //Register
            std::cout << "Register : " << std::endl;
            query = Qregister();
            c_query = new char[query.length() + 1];
            strcpy(c_query, query.c_str());
            requests.push_back(c_query);
            char* rep = pollResponses(responses);
            while(rep == NULL) {
                rep = pollResponses(responses);
            }
            while((int)rep[0] == 0)
            {
                query = Qregister();
                c_query = new char[query.length() + 1];
                strcpy(c_query, query.c_str());
                requests.push_back(c_query);
                rep = pollResponses(responses);
                while(rep == NULL) {
                    rep = pollResponses(responses);
                }
            }
        }

        if (query == "infos") {
            std::cout << "Infos : " << std::endl;
            int n = 32, nread = 0;
            query = '5'+std::to_string(n)+'|'+std::to_string(nread);
            c_query = new char[query.length() + 1];
            strcpy(c_query, query.c_str());

            app_mutex.lock();
            requests.push_back(c_query);
            std::cout << "Requête envoyée 2" << std::endl;
            app_mutex.unlock();

            char* rep = nullptr;
            while (rep == nullptr) {
                // std::cout << "attente" << std::endl;
                rep = pollResponses(responses);
            }
            std::cout << "=========XXXXXXXXXXXXXXXXXXXXXXXX======" << std::endl;

            // rep = "resp"+len(titre)+len(descr)+len(img)+titre+descr+img...    
            int i = 8;
            int size = (int)*rep;
            std::cout << size << std::endl;
            while(i < size) {
                int sizeTitle = rep[i];
                int sizeDesc = (rep[i+1] << 8) + rep[i+2];
                int sizeImg = (rep[i+3] << 16) + (rep[i+4] << 8) + rep[i+5];

                std::string title(&rep[i+6], sizeTitle);
                std::string descr(&rep[i+6+sizeTitle], sizeDesc);
                std::string img(&rep[i+6+sizeTitle+sizeDesc], sizeImg);
                i += 6+sizeTitle+sizeDesc+sizeImg;

                std::cout
                    << "Response of client : \nTitre : " << title
                    << "\nDescr : " << descr
                    << "\nImg : " << img
                    << std::endl;
                
            }
        }

    }

    gatewayThread.join();
    std::cout << "End of session" << std::endl;
    return EXIT_SUCCESS;
}

