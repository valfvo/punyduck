#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Mutex mutex;
bool threadRun;

class Render : public Thread
{
    // Cette classe threadée va s'occuper des calculs, afin de ne pas bloquer l'affichage
    public:

    Render(Image** _im, unsigned int _zoom, unsigned int _max_iteration, bool _inColor=false)
    {
        *_im=&im; //On utilise un pointeur de pointeur afin que le thread principal puisse afficher l'image

        inColor=_inColor;

        x1=-2.1;
        x2=0.6;
        y1=-1.2;
        y2=1.2;
        zoom=_zoom;
        iteration_max=_max_iteration;

        image_x = static_cast<unsigned int>((x2 - x1) * zoom);
        image_y = static_cast<unsigned int>((y2 - y1) * zoom);

        std::cout << "Image size : (" << image_x << ";" << image_y << ")" << std::endl;

        im=Image
        im.create(image_x, image_y, Color::Black); //On crée une image vide (noire)
        threadRun=true; //Cette variable globale sert à stopper le thread lorsque l'on ferme la fenêtre.
    }

    private:

    virtual void Run() //La fonction principale du thread de rendu
    {
        for(unsigned int x=0;x<image_x&&threadRun;x++) //On parcourt l'axe des X
        {
            for(unsigned int y=0;y<image_y&&threadRun;y++) //On parcourt l'axe des Y
            {
                double c_r=x/static_cast<double>(zoom)+x1;
                double c_i=y/static_cast<double>(zoom)+y1;
                double z_r=0;
                double z_i=0;
                double i=0;

                do
                {
                    double tmp=z_r;
                    z_r=z_r*z_r-z_i*z_i+c_r;
                    z_i=2*z_i*tmp+c_i;
                    ++i;
                }
                while(z_r*z_r+z_i*z_i<4&&i<iteration_max&&threadRun);

                if(threadRun)
                {
                    mutex.lock(); //On verouille l'image, afin que les deux threads n'entrent pas en colision
                    if(inColor)
                    {
                        if(i!=iteration_max)
                        {
                            im.setPixel(x, y, Color(0, 0, static_cast<int>(i*255/iteration_max))); //On change la couleur du pixel
                        }
                    }
                    else
                    {
                        if(i==iteration_max)
                            im.setPixel(x, y, Color::White);
                    }
                    mutex.unlock(); //Et on dévérouille
                }
            }
        }
        if(threadRun)
        {
            //Si l'on est arrivé ici, c'est que l'on a calculé tout ce qui était calculable.
            std::cout << "Render is over (" << elapsed.getElapsedTime() << "s) ! Saving..."<<std::endl;
            im.saveToFile("Fractal.png");
            std::cout << "Saved in \"Fractal.png\""<<std::endl;
        }
        else
        {
            //Si on est là, c'est que le rendu a été stoppé prématurément par l'utilisateur.
            std::cout << "Rendering aborded."<<std::endl;
        }
    }

    double x1;
    double x2;
    double y1;
    double y2;
    unsigned int zoom;
    unsigned int iteration_max;

    unsigned int image_x;
    unsigned int image_y;

    Clock elapsed; //Cet objet servira à déterminer le temps de rendu à postériori

    bool inColor;

    Image im;
};

int main()
{
    //On crée la fenetre, on prépare le sprite et l'image...
    const unsigned int RESO_X=800;
    const unsigned int RESO_Y=600;
    RenderWindow App(VideoMode(RESO_X, RESO_Y, 32), "Fractales");
    App.SetFramerateLimit(60);

    Image* ima=NULL;

    Render rend(&ima, 2500, 500, true); //On créé l'objet du rendu, en lui donnant les paramètres de la fractale (zoom, itérations max, et couleur)

    Sprite spr;
    spr.SetImage(*ima);

    //Cet objet sert à limiter l'appel aux fonctions d'affichage, qui bloquent le thread de rendu à cause des mutexs.
    Clock clock;
    const float time = 0.25;

    //Cet objet sert à déterminer le zoom de la vue, la position de la caméra, etc. Elle n'a qu'une influence sur la fenêtre, la fractale est toujours la même
    View view(Vector2f(ima->GetWidth()/2,ima->GetHeight()/2), Vector2f(RESO_X/2,RESO_Y/2));
    App.SetView(view);

    //On lance le thread de rendu
    rend.Launch();

    while(App.IsOpened())
    {
        Event Event;
        while (App.GetEvent(Event)) //On parcourt la pile des évenements
        {
            if (Event.Type==Event::Closed)
            {
                App.Close();
            }
            else if(Event.Type==Event::MouseWheelMoved) //Zoom/Dézoom à la molette de souris
            {
                if(Event.MouseWheel.Delta>0)
                {
                    view.Zoom(1.5);
                }
                else
                {
                    view.Zoom(0.75f);
                }
            }
            else if(Event.Type==Event::KeyPressed) //Déplacement
            {
                if(Event.Key.Code == Key::Left)
                {
                    view.Move(-10,0);
                }
                else if(Event.Key.Code == Key::Right)
                {
                    view.Move(10,0);
                }
                else if(Event.Key.Code == Key::Up)
                {
                    view.Move(0,-10);
                }
                else if(Event.Key.Code == Key::Down)
                {
                    view.Move(0,10);
                }
            }
        }

        if(clock.getElapsedTime()>time) //Si suffisament de temps s'est écoulé depuis le dernier affichage
        {
            clock.Reset();
            mutex.lock(); //On verouille l'image
            App.Draw(spr); //On l'affiche
            App.Display(); //On rafraichit l'écran
            mutex.unlock(); //On rend la main au thread de rendu
        }
    }

    threadRun=false; //Avant de quitter, il faut penser à stopper le thread de rendu.

    return 0;
}