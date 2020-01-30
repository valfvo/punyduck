#ifndef ENNEMIS_H_INCLUDED
#define ENNEMIS_H_INCLUDED
/*Fichier header de la classe Invader, contenant les déclarations de méthodes et d'attributs*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "fonctions.h"
#include "Projectile.h"

class Vaisseau; //On fait une forward declaration de la classe Vaisseau

class Invader
{
	/*Déclaration de la classe Invader, qui représentera les aliens (les ennemis).
	Ce qui est affiché à l'écran est son attribut "m_inv", qui est un sprite.
		Elle a également un objet de la classe Projectile en attribut*/
public: //Méthodes
    Invader();
    sf::Sprite getInv() const;
    int getWidth() const;
    int getHeight() const;
    sf::RectangleShape getProjectile() const;
    void setProjPosition(sf::Vector2f vect);
    void setInvPosition(sf::Vector2f);
    void moveInv(float x, float y);
    void create(const sf::Texture& t_invader1a);
    void resetProj();
    void tirer(bool *tir_alien, Vaisseau *player, sf::Sound& s_mort);

private: //Attributs
    sf::Sprite m_inv;
    int m_width;
    int m_height;
    Projectile m_projectile;
    sf::Vector2f m_position;
};

#endif // ENNEMIS_H_INCLUDED
