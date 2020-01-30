#include "Vaisseau.h"
#include "Invader.h"

using namespace sf;

inline Vaisseau::Vaisseau() : m_points(0), m_vies(3), m_positionVaisseau(m_rect.getPosition()), m_height(44), m_width(60)
{
	//Constructeur
}

inline void Vaisseau::create(const Texture& t_player)
{
	/*On applique la texture du vaisseau au sprite et on le place au milieu*/
    m_rect.setTexture(t_player);
    m_rect.setPosition(Vector2f(450, 600));
}

/*Méthodes accesseurs*/
inline int Vaisseau::getScore() const
{
    return m_points;
}

inline int Vaisseau::getLives() const
{
    return m_vies;
}

inline int Vaisseau::getWidth() const
{
    return m_width;
}

inline int Vaisseau::getHeight() const
{
    return m_height;
}

inline Sprite Vaisseau::getRect() const
{
    return m_rect;
}

inline RectangleShape Vaisseau::getProjectile() const
{
    return m_projectile.getProj();
}

inline void Vaisseau::resetKill()
{
	/*Méthode qui réinitialise le nombre d'aliens touchés*/
    m_projectile.resetKill();
}

inline void Vaisseau::setLives(int vies)
{
	/*Méthode qui change le nombre de vies*/
    m_vies = vies;
}

inline void Vaisseau::setPlayerPosition(Vector2f vect)
{
	/*Méthode qui change la position du sprite*/
    m_rect.setPosition(vect);
}

inline void Vaisseau::resetProj()
{
	/*Méthode qui réinitialise la position du sprite de l'attribut projectile*/
    m_positionVaisseau = m_rect.getPosition();
    m_projectile.setProjPosition(Vector2f(m_positionVaisseau.x + m_width/2 - m_projectile.getProjw()/2, m_positionVaisseau.y - m_projectile.getProjh()/2));
}

inline void Vaisseau::tirer(Invader tabInv[11][5], int tailleI, int tailleJ, bool *tir, Sound& s_explosion, float *sptr)
{
	/*Fonction qui gère les tirs du joueur*/
    m_projectile.moveProj(0, -10);
    if(m_projectile.fcollision(tabInv, tailleI, tailleJ, *this, s_explosion, sptr))
        *tir = false; //On teste s'il y a eu collision ou si le projectile a atteint la limite de la fenêtre
}

inline void Vaisseau::deplacement_vaisseau()
{
	/*Méthode qui gère le déplacement du joueur, donc celui du sprite*/
    m_positionVaisseau = m_rect.getPosition();
    if(Keyboard::isKeyPressed(Keyboard::Right) && m_positionVaisseau.x < 900)
    {
        m_rect.move(6, 0);
    }
    if(Keyboard::isKeyPressed(Keyboard::Left) && m_positionVaisseau.x > 50)
    {
        m_rect.move(-6, 0);
    }
}

inline void Vaisseau::score(int s)
{
	/*Méthode qui ajoute un certain nombre de points au score du joueur*/
    m_points += s;
}

inline void Vaisseau::live()
{
	/*Méthode qui retire une vie au joueur*/
    m_vies -= 1;
}
