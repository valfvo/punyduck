#include "litequarks.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>

std::mutex app_mutex;

bool pollResponses(std::vector<char*>& responses) {
    std::lock_guard<std::mutex> lock(app_mutex);
    if (!responses.empty()) {
        std::cout << responses[0] << std::endl;
        responses.erase(responses.begin());
        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    std::vector<char*> requests;
    std::vector<char*> responses;
    ClientGateway gateway(requests, responses);
    std::thread gatewayThread(std::ref(gateway));
    
    std::string query;
    for (;;) {
        std::cout << "Requête : ";
        std::cin >> query;

        char* c_query = new char[query.length() + 1];
        strcpy(c_query, query.c_str());
        requests.push_back(c_query);

        while (!pollResponses(responses));
    }

    gatewayThread.join();

    return EXIT_SUCCESS;
}