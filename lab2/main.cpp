/*
1.	Написать программу движения точки с расширяющимся и гаснущим следом. Управление движением точки осуществляется с помощью клавиш вправо, влево и вверх, вниз.
2.	Написать программу движения отрезка с расширяющимся и гаснущим следом. Управление движением отрезка осуществляется с помощью клавиш вправо, влево и вверх, вниз.
Все основные примитивы – позиция, скорость, отрезок должны использовать общий класс геометрический вектор. Операции с векторами реализовать с помощью операторов. 
*/

#include "vector.h"
#include "point.h"
#include "mote.h"
#include "head.h"
#include "scene.h"
#include <unistd.h>

int main(){
    Scene scene(Head({MAX_WIDTH / 2, MAX_HEIGHT / 2}, {0, 0}));
    while (scene.processInput()){
        scene.update(1);
        scene.draw();
        usleep(50000);
    }
    return 0;
}

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab2 && clang++ -std=c++17 main.cpp -o lab2.o && ./lab2.o
//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab2 && clang++ -std=c++17 vector.cpp point.cpp mote.cpp head.cpp scene.cpp main.cpp -o lab2.o && ./lab2.o