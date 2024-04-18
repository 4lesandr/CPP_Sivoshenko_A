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

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab2 && clang++ -std=c++17 vector.cpp point.cpp mote.cpp head.cpp scene.cpp main.cpp -o lab2.o && ./lab2.o