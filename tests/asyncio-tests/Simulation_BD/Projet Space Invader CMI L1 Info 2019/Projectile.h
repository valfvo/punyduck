#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED
/*Fichier header de la classe Projectile, contenant les déclarations de méthodes et d'attributs*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "fonctions.h"

class Invader; //On fait une forward declaration de la classe Invader
class Vaisseau; //On fait une forward declaration de la classe Invader

class Projectile
{
	/*Déclaration de la classe Projectile qui représentera les projectiles (comme son nom l'indique).
	C'est également elle qui gèrera les collisions et le nombre d'aliens touchés.
		Ce qui est affiché à l'écran est son attribut "m_proj", qui est un RectangleShape*/
public: //Méthodes
    Projectile();
    void setProjPosition(sf::Vector2f);
    void resetKill();
    void moveProj(float x, float y);
    sf::RectangleShape getProj() const;
    int getProjw() const;
    int getProjh() const;
    bool fcollision(Invader tabInv[11][5], int tailleI, int tailleJ,Vaisseau &player, sf::Sound& s_explosion, float *sptr);
    bool collision_tirAlien(Vaisseau *player, sf::Sound& s_mort);

private: //Attributs
    int m_width;
    int m_height;
    int m_kill;
    sf::RectangleShape m_proj;
    sf::Vector2f m_position;
};

#endif // PROJECTILE_H_INCLUDED
