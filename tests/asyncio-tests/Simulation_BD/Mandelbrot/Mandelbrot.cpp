#ifdef SFML_STATIC
#endif // SFML_STATIC

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include<cmath>

using namespace sf;

std::streambuf* errlog = sf::err().rdbuf(NULL);

class Complexe
{
    public:
    //Attributs
    float reel;
    float imag;
    float module;

    //Constructeur
    Complexe(): reel(0), imag(0), module(0) {}
    Complexe(float x, float y): reel(x), imag(y), module(sqrt(reel*reel + imag*imag)) {}

    //Surcharge des opérateurs
    Complexe& operator=(const Complexe &z)
    {
        reel = z.reel;
        imag = z.imag;
        module = z.module;
        return *this;
    }
    Complexe operator+(const Complexe &z) const
    {
        Complexe c(*this);
        c.reel += z.reel;
        c.imag += z.imag;
        c.module = sqrt(c.reel*c.reel + c.imag*c.imag);
        return c;
    }
    Complexe operator*(const Complexe &z) const
    {
        Complexe c(*this);
        c.reel = reel*z.reel - imag*z.imag;
        c.imag = reel*z.imag + imag*z.reel;
        c.module = sqrt(c.reel*c.reel + c.imag*c.imag);
        return c;
    }
};

struct Mandel
{
    Sprite rendu;
    Texture rendu_texture;
    int width, height;
    float x1, x2, y1, y2;
    int max_iteration;
};

void updateRendu(Mandel& mandel, float zoom, int mx, int my)
{
    Complexe c, z;
    int i;
    Image rend;
    rend.create(mandel.width, mandel.height, Color::Black);
    // float lx = (mandel.x2 - mandel.x1) * (1 - zoom);
    // float ly = (mandel.y2 - mandel.y1) * (1 - zoom); std::cout << "pourventage = ";
    mandel.x1 *= 1 - zoom; /** ((float)(mandel.width-mx)/mandel.width); std::cout << ((float)(mandel.width-mx)/mandel.width) << ", ";*/
    mandel.x2 *= 1 - zoom; /**(1-(float)(mandel.width-mx)/mandel.width); std::cout << (1-(float)(mandel.width-mx)/mandel.width) << ", ";*/
    mandel.y1 *= 1 - zoom; /**((float)(mandel.height-my)/mandel.height); std::cout << ((float)(mandel.height-my)/mandel.height) << ", ";*/
    mandel.y2 *= 1 - zoom; /**(1-(float)(mandel.height-my)/mandel.height); std::cout << (1-(float)(mandel.height-my)/mandel.height) << std::endl;;*/
    // mandel.x1 *= 1 - zoom * ((float) mx / mandel.width);
    // mandel.x2 *= 1 - zoom * ((float) (mandel.width - mx) / mandel.width);
    // mandel.y1 *= 1 - zoom * ((float) my / mandel.height);
    // mandel.y2 *= 1 - zoom * ((float) (mandel.height - my) / mandel.height);
    float zoomX = (float) mandel.width / (mandel.x2 - mandel.x1);
    float zoomY = (float) mandel.height / (mandel.y2 - mandel.y1);


    for(int x = 0; x < mandel.width; ++x)
    {
        for(int y = 0; y < mandel.height; ++y)
        {
            c = {x / zoomX + mandel.x1, y / zoomY + mandel.y1};
            z = {0, 0};
            i = 0;
            while(z.module < 2 && i < mandel.max_iteration)
            {
                z = z*z + c;
                i++;
            }

            if(i != mandel.max_iteration)
            {
                // float mix = (float) i / mandel.max_iteration;
                // Uint8 g = 255 * (1 - mix);
                // Uint8 b = 255 * mix;
                rend.setPixel(x, y, Color(0, 0, i*225/mandel.max_iteration));
            }
        }
    }
    mandel.rendu_texture = Texture();
    if(!mandel.rendu_texture.loadFromImage(rend))
    {
        //erreur
    }
    mandel.rendu.setTexture(mandel.rendu_texture);
    //rend.saveToFile("Mandelbrot_blue.png");
}

int main()
{
    RenderWindow window;
    float xmin = -2.1, xmax = 0.6, ymin = -1.2, ymax = 1.2;
    float zoom = 400, move = 0.1;
    int width = (xmax-xmin)*zoom;
    int height = (ymax-ymin)*zoom;
    Vector2u size(width, height);
    window.create(VideoMode(width, height), "Mandelbrot's set");
    int max_iteration = 25;
    Mandel mandel = {Sprite(), Texture(), width, height, xmin, xmax, ymin, ymax, max_iteration};
    updateRendu(mandel, 0, width/2, height/2);
    window.setFramerateLimit(30);
    int var = 0.01;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                {
                    updateRendu(mandel, 0.1, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                    ++mandel.max_iteration;
                    move -= var;
                    if(move-var == 0) var /= 10;
                    window.clear();
                }
                else if (event.mouseWheelScroll.delta < 0)
                {
                    updateRendu(mandel, -0.1, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                    --mandel.max_iteration;
                    move += var;
                    if(move == 10*var) var *= 10;
                    window.clear();
                }
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Left))
        {
            mandel.x1 -= move;
            mandel.x2 -= move;
            updateRendu(mandel, 0, 0, 0);
            window.clear();
        }
        if(Keyboard::isKeyPressed(Keyboard::Right))
        {
            mandel.x1 += move;
            mandel.x2 += move;
            updateRendu(mandel, 0, 0, 0);
            window.clear();
        }
        if(Keyboard::isKeyPressed(Keyboard::Up))
        {
            mandel.y1 -= move;
            mandel.y2 -= move;
            updateRendu(mandel, 0, 0, 0);
            window.clear();
        }
        if(Keyboard::isKeyPressed(Keyboard::Down))
        {
            mandel.y1 += move;
            mandel.y2 += move;
            updateRendu(mandel, 0, 0, 0);
            window.clear();
        }

        window.draw(mandel.rendu);
        window.display();
    }

    return 0;
}