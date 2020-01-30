del Mandelbrot.exe
del Mandelbrot.o
g++ -c Mandelbrot.cpp -ID:\Travail\SFML-2.5.1\include
g++ Mandelbrot.o -o Mandelbrot.exe -LD:\Travail\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system
pause