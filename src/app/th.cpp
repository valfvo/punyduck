#include "litequarks.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <typeinfo>

std::mutex app_mutex;

char* pollResponses(std::vector<char*>& responses) {
    std::lock_guard<std::mutex> lock(app_mutex);
    if (!responses.empty()) {
        std::cout << "Response of py = " << responses[0] << std::endl;
        char* save = responses[0];
        responses.erase(responses.begin());
        return save;
    }
    return NULL;
}

std::string Qregister() {
    std::string login, password, email, query;
    std::cout << "Entrez un login :";
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
    std::vector<char*> requests;
    std::vector<char*> responses;
    ClientGateway gateway(requests, responses);
    std::thread gatewayThread(std::ref(gateway));
    
    std::string query;
    char* c_query;
    bool connected = true;
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
            while (responses.empty());
            std::cout << "Response of client : " << responses[0] << ", type : " << typeid(responses[0]).name() << std::endl;
            while(strcmp(responses[0], "0") == 0)
            {
                responses.erase(responses.begin());
                query = Qregister();
                c_query = new char[query.length() + 1];
                strcpy(c_query, query.c_str());
                requests.push_back(c_query);
                while (responses.empty());
                std::cout << "Response of client : " << responses[0] << ", type : " << typeid(responses[0]).name() << std::endl;
            }
            responses.erase(responses.begin());
        }

    }

    gatewayThread.join();
    std::cout << "End of session" << std::endl;
    return EXIT_SUCCESS;
}