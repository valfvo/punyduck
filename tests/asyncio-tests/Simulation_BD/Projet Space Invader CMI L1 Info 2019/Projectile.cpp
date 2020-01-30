#include "Projectile.h"
#include "Vaisseau.h"
#include "Vaisseau.cpp"
#include "Invader.h"

using namespace std;
using namespace sf;

Projectile::Projectile() : m_width(5), m_height(20), m_kill(0)
{
	/*On initialise la taille du projectile dans le constructeur*/
    m_proj.setSize(Vector2f(m_width, m_height));
}

void Projectile::setProjPosition(Vector2f vect)
{
	/*Méthode qui change la position du projectile*/
    m_proj.setPosition(vect);
}

void Projectile::resetKill()
{
	/*Méthode qui réinitialise le nombre d'alien morts*/
    m_kill = 0;
}

void Projectile::moveProj(float x, float y)
{
	/*Méthode qui déplace le projectile*/
    m_proj.move(x, y);
}

/*Méthode accesseurs*/
RectangleShape Projectile::getProj() const
{
    return m_proj;
}

int Projectile::getProjw() const
{
    return m_width;
}

int Projectile::getProjh() const
{
    return m_height;
}

bool Projectile::fcollision(Invader tabInv[11][5], int tailleI, int tailleJ, Vaisseau& player, Sound& s_explosion, float *sptr)
{
	/*Méthode qui gère les collisions pour les tirs du joueur*/
    Vector2f posInv;
    m_position = m_proj.getPosition();
    for(int i(0); i < tailleI; i++)
    {
        for(int j(0); j < tailleJ; j++)
        {
            posInv = tabInv[i][j].getInv().getPosition();
            if(m_position.x < posInv.x + tabInv[i][j].getWidth() && m_position.x + m_width > posInv.x && m_position.y <= posInv.y + tabInv[i][j].getHeight() && m_position.y >= posInv.y)
            {
            	//Si le projectile touche un alien, on joue le son de mort alien, on incrémente le nombre de mort alien et on augmente le score d'un nombre de point en fonction du type d'alien touché
                s_explosion.play();
                m_kill += 1;
                tabInv[i][j].setInvPosition(Vector2f(tabInv[i][j].getInv().getPosition().x, -2000));
                if(j==3 || j==4)
                    player.score(10);
                if(j==2 || j==1)
                    player.score(20);
                if(j==0)
                    player.score(40);
                if(m_kill == 55)
                {
                    m_kill = 0;
                    createInvaders(sptr);
                }
                return true;
            }
            else
            {
            	//Sinon, si le projectile atteint la limite de la fenêtre on renvoie true également
                if(m_position.y <= 0)
                    return true;
            }
        }
    }
    return false; //Si le projectile n'a pas touché d'alien et n'est pas hors de l'écran (= le projectile est encore en chemin) on renvoie false
}

bool Projectile::collision_tirAlien(Vaisseau *player, Sound& s_mort)
{
	/*Méthode qui gère les collision pour les tirs aliens*/
    m_position = m_proj.getPosition();
    if(m_position.x <= player->getRect().getPosition().x + player->getWidth() && m_position.x - m_width >= player->getRect().getPosition().x && m_position.y + m_height >= player->getRect().getPosition().y && m_position.y + m_height <= player->getRect().getPosition().y + player->getHeight())
    {
    	//Si le projectile touche le joueur, on replace le joueur au centre, on lui enlève une vie et on joue le son du mort
        player->setPlayerPosition(Vector2f(450, 600));
        player->live();
        s_mort.play();
        return true;
    }
    else
    {
        //Sinon, si le projectile atteint la limite de la fenêtre on renvoie true également
        if(m_position.y > 1000)
            return true;
    }
    return false; //Si le projectile n'a pas touché d'alien et n'est pas hors de l'écran (= le projectile est encore en chemin) on renvoie false
}
