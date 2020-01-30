#include "Invader.h"
#include "Vaisseau.cpp"

using namespace sf;
using namespace std;

Invader::Invader() : m_width(40), m_height(25)
{
	//Constructeur
}

/*Méthodes accesseurs*/
Sprite Invader::getInv() const
{

    return m_inv;
}

int Invader::getWidth() const
{
    return m_width;
}

int Invader::getHeight() const
{
    return m_height;
}

RectangleShape Invader::getProjectile() const
{
    return m_projectile.getProj();
}

void Invader::setProjPosition(Vector2f vect)
{
	/*Méthode qui change la position du sprite de l'attribut projectile*/
    m_projectile.setProjPosition(vect);
}

void Invader::setInvPosition(Vector2f vect)
{
	/*Méthode qui change la position du sprite*/
    m_inv.setPosition(vect);
}

void Invader::moveInv(float x, float y)
{
	/*Méthode qui déplace le sprite*/
    m_inv.move(x, y);
}

void Invader::create(const Texture& t_invader1a)
{
	/*Méthode qui applique une texture entrée en paramètre au sprite*/
    m_inv.setTexture(t_invader1a);
}

void Invader::resetProj()
{
	/*Méthode qui réinitialise la position du sprite de l'attribut projectile*/
    m_position = m_inv.getPosition();
    m_projectile.setProjPosition(Vector2f(m_position.x + m_width/2 - m_projectile.getProjw()/2, m_position.y - m_projectile.getProjh()/2));
}

void Invader::tirer(bool *tir_alien, Vaisseau *player, Sound& s_mort)
{
	/*Méthode qui gère les tirs aliens*/
    m_projectile.moveProj(0, 10);
    if(m_projectile.collision_tirAlien(player, s_mort))
        *tir_alien = false; //On teste s'il y a eu collision ou si le projectile a atteint la limite de la fenêtre
}
