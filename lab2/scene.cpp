#include <iostream>
#include <vector>
#include "mote.h"
#include "head.h"
#include "scene.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>



enum class Symbol{
    //@, r, !, :, .
    at = '@',
    r = '/',
    exclamation = '!',
    colon = ':',
    dot = '.'    
};

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

Scene::Scene(const Head& head): head(head){}

void Scene::update(double dt){
    head.move(dt);
    for (auto& mote: head.emit()){
        motes.push_back(mote);
    }
    for (auto it = motes.begin(); it != motes.end();){
        if (!it->update(dt)){
            it = motes.erase(it);
        } else {
            ++it;
        }
    }
}

void Scene::draw(){
    int width = MAX_WIDTH;
    int height = MAX_HEIGHT;
    
    std::vector<std::string> canvas(height, std::string(width, ' '));

    for (auto& mote: motes){
        Vector pos = mote.getPosition();
    
        int x = static_cast<int>(pos.getX());
        int y = static_cast<int>(pos.getY());
        
        if (x >= 0 && x < width && y >= 0 && y < height) {
    
            if(mote.getLifeTime() <= 1){
                canvas[y][x] = static_cast<char>(Symbol::r);
            }
            else if(mote.getLifeTime() <= 2){
                canvas[y][x] = static_cast<char>(Symbol::exclamation);
            }
            else if(mote.getLifeTime() <= 3){
                canvas[y][x] = static_cast<char>(Symbol::colon);
            }
            else if(mote.getLifeTime() <= 4){
                canvas[y][x] = static_cast<char>(Symbol::dot);
            }
        }
    }
    Vector pos = head.getPosition();
    int x = static_cast<int>(pos.getX());
    int y = static_cast<int>(pos.getY());
    if (x >= 0 && x < width && y >= 0 && y < height) {
        canvas[y][x] = static_cast<char>(Symbol::at);   
    }
    for (int i = 0; i < height; i++){
        std::cout << canvas[i] << std::endl;
    }
    std::cout << std::endl;
}

bool Scene::processInput(){
    if (kbhit()){
        char c = getchar();
        switch (c){
            case 'w':
                head.accelerate({0, -1});
                break;
            case 's':
                head.accelerate({0, 1});
                break;
            case 'a':
                head.accelerate({-1, 0});
                break;
            case 'd':
                head.accelerate({1, 0});
                break;
            case 'q':
                return false;
                break;
        }
    }
    return true;
}
