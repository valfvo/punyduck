#ifdef SFML_STATIC
#endif // SFML_STATIC

//Inclusion des biblioth�ques et des classes cr��es
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include "Vaisseau.h"
#include "Vaisseau.cpp"
#include "Invader.h"
#include "fonctions.h"

using namespace std;
using namespace sf;

/*D�claration des variables globales*/
RenderWindow fenetre; //La fen�tre
Vaisseau player; //Le joueur
int const tailleI(11); //Dimensions x du tableau d'alien
int const tailleJ(5); //Dimensions y du tableau d'alien
Invader tabInv[tailleI][tailleJ]; //Le tableau d'alien, contenant 11x5 objet de la classe Invader
Font police;
string textScore, textVies;
Text textscore, textvies;
Texture t_background, t_player, t_invader1a, t_invader2a, t_invader3a;
Sprite sp_background;
SoundBuffer bf_tir, bf_tiralien, bf_mort, bf_explosion;
Sound s_tir, s_tiralien, s_mort, s_explosion;
Music ms_title, ms_play;
time_t heure_base = time(NULL);
time_t heure_new;
bool jeu(false);

void createInvaders(float *sptr)
{
	/*Fonction servant � initialiser la position de tous les aliens*/
    *sptr = 0.5;
    for(int i(0); i < tailleI; i++)
    {
        for(int j(0); j < tailleJ; j++)
        {
        	//On applique une texture diff�rente en fonction de la ligne de l'alien
            if(j==3 || j==4)
                tabInv[i][j].create(t_invader1a);
            if(j==1 || j==2)
                tabInv[i][j].create(t_invader2a);
            if(j==0)
                tabInv[i][j].create(t_invader3a);
            //On met la position de chaque alien en fonction de sa ligne et de sa colonne
            tabInv[i][j].setInvPosition(Vector2f((i*60 + 170), (j*50 + 100)));
        }
    }
}

void deplacementInvader(bool droite, float speed)
{
	/*Fonction qui g�re le d�placement des aliens, en fonction de leur sens de d�placement (droite ou gauche)*/
    if(droite)
    {
        for(int i(0); i < tailleI; i++)
        {
            for(int j(0); j < tailleJ; j++)
            {
                tabInv[i][j].moveInv(speed, 0);
            }
        }
    }
    else if(!droite)
    {
        for(int i(0); i < tailleI; i++)
        {
            for(int j(0); j < tailleJ; j++)
            {
                tabInv[i][j].moveInv(-speed, 0);
            }
        }
    }
}

bool sensDeplacement(bool droite, float *sptr)
{
	/*Fonction qui renvoie le sens de d�placement des aliens (true = droite, false = gauche)
		On change de sens quand le groupe arrive � une extr�mit� de l'�cran*/
    if(tabInv[10][4].getInv().getPosition().x + tabInv[10][4].getWidth() >= 960)
    {
        for(int i(0); i < tailleI; i++)
        {
            for(int j(0); j < tailleJ; j++)
            {
                tabInv[i][j].moveInv(0, tabInv[i][j].getHeight()); //On baisse leur position une fois arri�s au bout
            }
        }
        *sptr *= 1.3; //On incr�mente la vitesse des aliens une fois sur 2
        return false;
    }

    else if(tabInv[0][0].getInv().getPosition().x <= 40)
    {
        for(int i(0); i < tailleI; i++)
        {
            for(int j(0); j < tailleJ; j++)
            {
                tabInv[i][j].moveInv(0, tabInv[i][j].getHeight()); //On baisse leur position une fois arri�s au bout
            }
        }
        return true;
    }

    else return droite;
}

void display(bool tir, bool tir_alien)
{
	/*Fonction qui affiche les objets de base du jeu, prend en param�tre 2 bool�ens qui indiquent si la fonctions doit afficher les projectiles*/
    fenetre.clear();
    fenetre.draw(sp_background);
    fenetre.draw(player.getRect());
    if(tir)
    {
        fenetre.draw(player.getProjectile()); //Si le joueur tire, afficher son projectile
    }

    if(tir_alien)
    {
        for(int i(0); i < tailleI; i++)
        {
            for(int j(0); j < tailleJ; j++)
            {
                fenetre.draw(tabInv[i][j].getProjectile()); //Si un alien a tir�, afficher les projectiles aliens
            }
        }
    }
    for(int i(0); i < tailleI; i++)
    {
        for(int j(0); j < tailleJ; j++)
        {
            fenetre.draw(tabInv[i][j].getInv());
        }
    }
    fenetre.draw(textscore);
    fenetre.draw(textvies);
    fenetre.display();
}

void replay(bool *dptr, float *sptr)
{
	/*Fonction appel�e en fin du jeu, qui affiche un �cran Game Over et remet les statistique � z�ro (aliens tu�s, score, positions, etc.)*/
    bool rejouer(false);
    //On r�initialise les diff�rentes variables
    *dptr = true;
    *sptr = 0.5;
    player.resetKill();
    player.setLives(3);
    player.score(-player.getScore());
    player.setPlayerPosition(Vector2f(450, 600));
    Texture t_gameover;
    Sprite sp_gameover;
    Event event;
    if(!t_gameover.loadFromFile("Images/gameover.png"))
        {}//Erreur
    sp_gameover.setPosition(Vector2f(288, 25));
    sp_gameover.setTexture(t_gameover);
    while(!rejouer)
    {
    	//Tant que le joueur n'a pas appuy� sur une touche, on affiche l'�cran de Game Over
        fenetre.clear();
        fenetre.draw(sp_background);
        fenetre.draw(sp_gameover);
        fenetre.display();
        while(fenetre.pollEvent(event))
        {
            if(event.type == Event::KeyReleased)
            {
                fenetre.setFramerateLimit(60);
                rejouer = true;
            }
        }
    }
    createInvaders(sptr);
    ms_play.stop();
    ms_title.play();
}

//Des variables globales qui repr�senteront des coordonn�es al�atoires pour cibler un alien
int alien_x, alien_y;

void jouer(bool *dptr, float *sptr, bool *tir, bool *tir_alien)
{
	/*Fonction principale qui g�re le syst�me de jeu*/
    heure_new = time(NULL); //On intialise une 2e variable temps qui prend l'heure du syst�me comme valeur
    double temps;
    srand(time(0));
    temps = rand() % 5; //On prend un nombre al�atoire entre 0 et 5

    //Gestion d�placement
    deplacementInvader(*dptr, *sptr);
    *dptr = sensDeplacement(*dptr, sptr);
    player.deplacement_vaisseau();

    if(Keyboard::isKeyPressed(Keyboard::Space) && !*tir)
    {
    	//Si on appuie sur espace et qu'un tir n'est pas d�j� en cours
        *tir = true;
        player.resetProj(); //On r�initialise la position du projectile
        s_tir.play(); //On joue le bruitage du tir
    }

    if(*tir)
        player.tirer(tabInv, tailleI, tailleJ, tir, s_explosion, sptr); //Si on a appuy� sur espace, appeler la m�thode tirer du joueur

    if(difftime(heure_new, heure_base) > temps && !*tir_alien)
    {
    	/*Ici, on v�rifie si la diff�rence de temps entre le timer d�finit au d�but de la fonction et celui
    		d�finit au d�but du programme est sup�rieure au nombre al�atoire g�n�r�.
    		Si c'est le cas on fait tirer un alien al�atoire.
    		En clair, on attend un temps al�atoire (inf�rieur � 5 secondes) pour faire tirer un alien al�atoire*/
        heure_base = time(NULL);
        *tir_alien = true;
        srand(time(0));
        alien_x = rand() % 11;
        alien_y = rand() % 5;
        for(int x(0); x < tailleI; x++)
        {
            for(int y(0); y < tailleJ; y++)
            {
                tabInv[x][y].setProjPosition(Vector2f(-100, -100)); //On cache les projectiles de tous les aliens
            }
        }
        tabInv[alien_x][alien_y].resetProj();
        if(tabInv[alien_x][alien_y].getInv().getPosition().y < 0)
            *tir_alien = false; //Etant donn� que lorsqu'un alien est tu�, il ne dispara�t pas mais est seulement d�plac� au dessus de l'�cran, on v�rifie que les aliens hors-�cran ne tirent pas
        else
            s_tiralien.play();
    }
    if(*tir_alien)
        tabInv[alien_x][alien_y].tirer(tir_alien, &player, s_mort); //Si la structure conditionnelle au dessus est vraie, on fait tirer l'alien

    if(perdu())
    {
        fenetre.setFramerateLimit(1); /*J'ai baiss� la limite d'images par seconde afin d'obliger le programme � "patienter" et d'�viter que le joueur appuie sur une touche au moment de
        								mourrir et n'ait pas le temps de voir l'�cran Game Over*/
        replay(dptr, sptr);
        jeu = false;
    }

    //On actualise le score et le nombre de vies � chaque tour de boucle
    textScore = "Score : " + to_string(player.getScore());
    textVies = "Lives : " + to_string(player.getLives());
    textscore.setString(textScore);
    textvies.setString(textVies);
}

bool perdu()
{
	/*Fonction qui v�rifie si le joueur a perdu*/
    for(int i(0); i < tailleI; i++)
    {
        for(int j(0); j < tailleJ; j++)
        {
            if(tabInv[i][j].getInv().getPosition().y > 550)
                return true; //Si les aliens arrivent en bas, le joueur perd
        }
    }

    if(player.getLives() == 0)
            return true; //Si le joueur se fait toucher 3 fois, il perd

    return false;
}

int main()
{
	//Variables
    Event event;
    bool information(false);
    bool tir(false);
    bool tir_alien(false);
    bool droite(true);
    bool *dptr(&droite);
    float speed(0.5);
    float *sptr = &speed;
    Vector2i posMouse;
    Texture t_main, t_info;
    Sprite sp_main, sp_info;

    //Cr�ation fen�tre + formes
    fenetre.create(VideoMode(1000,700), "Space Invader");
    fenetre.setFramerateLimit(60);

    /*On charge les textures et les sons*/
    if(t_main.loadFromFile("Images/main.png")) //Ecran d'accueil
        {}//Erreur
    sp_main.setTexture(t_main);
    if(t_info.loadFromFile("Images/Informations.png")) //Ecran d'informations
        {}//Erreur
    sp_info.setTexture(t_info);
    if(!t_background.loadFromFile("Images/background.png"))
        {}//Erreur
    sp_background.setTexture(t_background);
    sp_background.setColor(Color(255, 255, 255, 100));
    textscore.setFont(police);
    textvies.setFont(police);
    if(!ms_title.openFromFile("Images/title.wav")) //Musique de l'�cran titre
        {}//Erreur
    ms_title.setVolume(20);
    ms_title.setLoop(true);
    ms_title.play();
    if(!ms_play.openFromFile("Images/play.wav")) //Musique pendant le jeu
        {}//Erreur
    ms_play.setVolume(20);
    ms_play.setLoop(true);
    if (!police.loadFromFile("Images/OpenSans_Regular_webfont.ttf"))
    	{}//Erreur
    textvies.setPosition(Vector2f(850, 0));
    if(!t_player.loadFromFile("Images/player.png"))
	    {}//Erreur
    player.create(t_player); //On initialise le sprite du joueur
    if(!t_invader1a.loadFromFile("Images/invader1a.png"))
	    {}//Erreur
    if(!t_invader2a.loadFromFile("Images/invader2a.png"))
	    {}//Erreur
    if(!t_invader3a.loadFromFile("Images/invader3a.png"))
	    {}//Erreur
    createInvaders(sptr); //On initialise la position des aliens
    if(!bf_tir.loadFromFile("Images/Tir.wav")) //Son du tir
	    {}//Erreur
    s_tir.setBuffer(bf_tir);
    if(!bf_tiralien.loadFromFile("Images/Shoot.wav")) //Son du tir alien
	    {}//Erreur
    s_tiralien.setBuffer(bf_tiralien);
    if(!bf_mort.loadFromFile("Images/Explosion.wav")) //Son de la mort du joueur
	    {}//Erreur
    s_mort.setBuffer(bf_mort);
    if(!bf_explosion.loadFromFile("Images/Powerup.wav")) //Son de la mort d'un alien
	    {}//Erreur
    s_explosion.setBuffer(bf_explosion);
    s_tir.setVolume(80);
    s_tiralien.setVolume(10);
    s_explosion.setVolume(10);
    s_mort.setVolume(20);

    //Tant que la fen�tre est ouverte
    while(fenetre.isOpen())
    {
        //Test des �v�nements
        while(fenetre.pollEvent(event))
        {
            //Si l'event est le bouton fermer
            if(event.type == Event::Closed)
                fenetre.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
        	//Si on appuie sur echap, on revient au menu principal
        	if(jeu)
            {
                *sptr = 0.5;
                *dptr = true;
                tir = false;
                tir_alien = false;
                player.resetKill();
                player.setLives(3);
                player.score(-player.getScore());
                player.setPlayerPosition(Vector2f(450, 600));
                player.resetProj();
                createInvaders(sptr);
                for(int x(0); x < tailleI; x++)
                {
                    for(int y(0); y < tailleJ; y++)
                    {
                        tabInv[x][y].setProjPosition(Vector2f(-100, -100));
                    }
                }
                ms_play.stop();
                ms_title.play();
            }
            jeu = false;
            information = false;
        }

        if(jeu)
        {
            jouer(dptr, sptr, &tir, &tir_alien);
            display(tir, tir_alien);
        }

        else
        {
            if(information)
            {
                fenetre.clear();
                fenetre.draw(sp_info);
                fenetre.display();
            }
            else
            {
            	//Si on n'est ni en jeu ni dans le menu information, afficher l'�cran titre et g�rer les clics souris
                fenetre.clear();
                fenetre.draw(sp_main);
                fenetre.display();
                posMouse = Mouse::getPosition(fenetre);
                if(Mouse::isButtonPressed(Mouse::Left))
                {
                	//On g�re les clics de la souris sur les diff�rentes options (start, information et quit)
                    if(posMouse.x > 360 && posMouse.x < 620 && posMouse.y > 360 && posMouse.y < 420) //Start
                        {
                            jeu = true;
                            ms_title.stop();
                            ms_play.play();
                        }
                    if(posMouse.x > 270 && posMouse.x < 730 && posMouse.y > 488 && posMouse.y < 518) //Information
                        information = true;
                    if(posMouse.x > 394 && posMouse.x < 599 && posMouse.y > 616 && posMouse.y < 671) //Quit
                        fenetre.close();
                }
            }
        }
    }

    return 0;
}
