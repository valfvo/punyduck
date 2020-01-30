#ifndef VAISSEAU_H_INCLUDED
#define VAISSEAU_H_INCLUDED
/*Fichier header de la classe Vaisseau, contenant les déclarations de méthodes et d'attributs*/

#include <SFML/Graphics.hpp>
#include "fonctions.h"
#include "Projectile.h"

class Invader; //On fait une forward declaration de la classe Invader

class Vaisseau
{
	/*Déclaration de la classe Vaisseau qui nous servira à créer notre joueur.
	Ce qui est affiché à l'écran est son attribut "m_rect", qui est un sprite.
		Elle a également un objet de la classe Projectile en attribut.*/
public: //Méthodes
    inline Vaisseau();
    inline void create(const sf::Texture& t_player);
    inline sf::Sprite getRect() const;
    inline sf::RectangleShape getProjectile() const;
    inline int getScore() const;
    inline int getLives() const;
    inline int getWidth() const;
    inline int getHeight() const;
    inline void resetKill();
    inline void setLives(int vie);
    inline void setPlayerPosition(sf::Vector2f vect);
    inline void resetProj();
    inline void tirer(Invader tabInv[11][5], int tailleI, int tailleJ, bool *tir, sf::Sound& s_explosion, float *sptr);
    inline void deplacement_vaisseau();
    inline void score(int s);
    inline void live();

private: //Attributs
    int m_points;
    int m_vies;
    sf::Vector2f m_positionVaisseau;
    sf::Sprite m_rect;
    int m_height;
    int m_width;
    Projectile m_projectile;
};

#endif // VAISSEAU_H_INCLUDED
