g++ -c main.cpp -ID:\Travail\SFML-2.5.1\include
g++ -c Invader.cpp -ID:\Travail\SFML-2.5.1\include
g++ -c Projectile.cpp -ID:\Travail\SFML-2.5.1\include
g++ -c Vaisseau.cpp -ID:\Travail\SFML-2.5.1\include
g++ main.o Invader.o Projectile.o Vaisseau.o -o Space_Invader.exe -LD:\Travail\SFML-2.5.1\lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system