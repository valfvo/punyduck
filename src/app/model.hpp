#pragma once

#include <string>
#include <litequarks/litequarks.hpp>


struct Project {
    int id;
    std::string nom;
    std::string tag;
    std::string desc;
    LQImageData img;
    std::string auteur;
    // float notes;
};

struct Logos {
    LQImageData search;
    LQImageData mosaique;
    LQImageData liste;
    LQImageData personne;
    LQImageData boutonEnvoi;
};

struct User {
    std::string nom;
    LQImageData imgProfile;
    LQImageData imgCouv;
    LQImageData boutonOffline;
    LQImageData boutonOnline;
};

void initModelsConstructors();
