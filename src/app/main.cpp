#include <iostream>
#include <vector>
#include <typeindex>
#include <queue>
#include <functional>
#include <unordered_map>
#include <litequarks/litequarks.hpp>

// #include <iostream>
// #include <cstring>
// #include <string>
// #include <thread>
// #include <vector>
// #include <mutex>
// #include <functional>
// #include <typeinfo>
// #include <utility>

// using namespace LQUnit;

int SCREEN_WIDTH  = 1280;
int SCREEN_HEIGHT = 720;

// LQColor BLACK (0x000000);
// LQColor WHITE (0xffffff);
// LQColor RED   (0xff0000);
// LQColor GREEN (0x00ff00);
// LQColor BLUE  (0x0000ff);

#define FVO_GREEN 0x1DBF73
// #define FVO_GREEN 0x25a65c
// #define FVO_GREY  0x283234
// #define FVO_GREY  0x282c34
#define FVO_GREY  0x36393F
// #define FVO_GREY2 0x283634
// #define FVO_GREY 0x282c34
// #define FVO_TEXT  0xacb2bf
// #define FVO_TEXT  0xe1e5ed
#define FVO_TEXT  0xdcddde

#define BACKGROUND_PRIMARY   0x36393f
#define BACKGROUND_SECONDARY 0x2f3136
#define BACKGROUND_TERNARY   0x202225

std::string Qregister() {
    std::string login, password, email, query;
    std::cout << "Entrez un login : ";
    std::cin >> login;
    std::cout << "Entrez un password :";
    std::cin >> password;
    while(password.length() < 8) {
        std::cout << "Mot de passe trop court (< 8 characteres),"
                     "veuillez en choisir un autre : ";
        std::cin >> password;
    }
    std::cout << "Entrez un email :";
    std::cin >> email;

    query = '1'+login+'|'+password+'|'+email;
    std::cout << "Query = " << query << std::endl;

    return query;
}

int main() {
    LQAppController::init();

    std::string query;
    char* c_query;
    bool connected = true;
    // char* rep;
    while (connected) {
        std::cout << "Requete : ";
        std::cin >> query;

        // TODO controller transmit request
        if (query == "exit") {
            std::cout << "Exit..." << std::endl;
            char* c = new char[1];
            c[0] = '0';
            LQAppController::pushQuery(c);
            connected = false;
        }

        else if (query == "register") {
            std::cout << "Register : " << std::endl;
            query = Qregister();
            c_query = new char[query.length() + 1];
            strcpy(c_query, query.c_str());
            LQAppController::pushQuery(c_query);
            char* rep = nullptr;
            while (rep == nullptr) {
                // rep = LQAppController::pollResponses();
            }
            while((int)rep[0] == 0) {
                query = Qregister();
                c_query = new char[query.length() + 1];
                strcpy(c_query, query.c_str());
                LQAppController::pushQuery(c_query);
                // rep = pollResponses(responses);
                // while(rep == NULL) {
                //     rep = pollResponses(responses);
                // }
            }
        }

        else if (query == "infos") {
            std::cout << "Infos : " << std::endl;
            int n = 32, nread = 0;
            query = '5'+std::to_string(n)+'|'+std::to_string(nread);
            c_query = new char[query.length() + 1];
            strcpy(c_query, query.c_str());

            LQAppController::pushQuery(c_query);
            std::cout << "Requête envoyée 2" << std::endl;

            char* rep = nullptr;
            while (rep == nullptr) {
                // std::cout << "attente" << std::endl;
                // rep = pollResponses(responses);
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
    // TODO lqInit();
    glfwInit();
    LQAppController::init();

    LQWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Punyduck");
    window.run();

    std::cout << "window closed" << std::endl;
    LQAppController::finalize();
    glfwTerminate();
    // TODO lqFinalize();

    return EXIT_SUCCESS;
}
