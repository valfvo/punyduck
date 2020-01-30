#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

/*Fichier contenant les signatures des fonctions du fichier main.cpp*/
void display(bool tir, bool tir_alien);
void createInvaders(float *sptr);
void deplacementInvader(bool droite, float speed);
bool sensDeplacement(bool droite, float *speed);
bool perdu();
void replay();
void jouer(bool *dptr, float *sptr, bool *tir, bool *tir_alien);

#endif // FONCTIONS_H_INCLUDED
