/*
1.	Написать программу движения точки с расширяющимся и гаснущим следом. Управление движением точки осуществляется с помощью клавиш вправо, влево и вверх, вниз.
2.	Написать программу движения отрезка с расширяющимся и гаснущим следом. Управление движением отрезка осуществляется с помощью клавиш вправо, влево и вверх, вниз.
Все основные примитивы – позиция, скорость, отрезок должны использовать общий класс геометрический вектор. Операции с векторами реализовать с помощью операторов. 
*/
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_HEIGHT 65
#define MAX_WIDTH 206

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

class Vector{
    private:
        double x;
        double y;
    public:
        Vector(double x, double y): x(x), y(y){}
        void setX(double x){ this->x = x; }
        void setY(double y){ this->y = y; }
        double getX() const { return x; }
        double getY() const { return y; }

        Vector operator+(const Vector& v) const{
            return Vector(x + v.x, y + v.y);
        }
        Vector operator-(const Vector& v) const{
            return Vector(x - v.x, y - v.y);
        }
        Vector operator*(double k) const{
            return Vector(x * k, y * k);
        }
        Vector operator/(double k) const{
            return Vector(x / k, y / k);
        }
        Vector operator-() const{
            return Vector(-x, -y);
        }
        

        double operator*(const Vector& v) const{
            return x * v.x + y * v.y;
        }
        double operator^(const Vector& v) const{
            return x * v.y - y * v.x;
        }
        double length() const{
            return sqrt(x * x + y * y);
        }
        double angle(const Vector& v) const{
            return atan2(*this ^ v, *this * v);
        }
        double distance(const Vector& v) const{
            return (*this - v).length();
        }

        Vector normalise() const{
            return *this / length();
        }
        Vector rotate(double angle) const{
            return Vector(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
        }
        Vector project(const Vector& v) const{
            return v * (*this * v) / (v * v);
        }
        Vector reflect(const Vector& v) const{
            return *this - v * 2 * (*this * v) / (v * v);
        }

        Vector perpendicular() const{
            return Vector(-y, x);
        }
};

class Point{
    private:
        Vector position;
        Vector velocity;
    public:
        Point(const Vector& position, const Vector& velocity): position(position), velocity(velocity){}
        void setPosition(const Vector& position){ this->position = position; }
        void setVelocity(const Vector& velocity){ this->velocity = velocity; }
        void move(double dt);
        Vector getPosition() const { return position; }
        Vector getVelocity() const { return velocity; }
};

//пылинка создается, затухает и исчезает
class Mote:public Point{
    private:
        int lifeTime;
        static inline const int maxLifeTime{5};
    public:
        Mote(const Vector& position, const Vector& velocity): Point(position, velocity), lifeTime(0){}
        int getLifeTime() const { return lifeTime; }
        bool update(double dt);
};

class Point_object: public Point{
    private:
        double const max_speed{5};
    public:
        Point_object(const Vector& position, const Vector& velocity): Point(position, velocity){}
        void accelerate(const Vector& acceleration);
        std::vector<Mote> emit();
};

class Segment: public Point{
    private:
        double length;
        static inline const int max_speed{5};
    public:
        Segment(const Vector& start, const double length, const Vector& velocity): Point(start, velocity) , length(length){}
        void accelerate(const Vector& acceleration);
        double getLength() const { return length; }
        std::vector<Mote> emit();
};

enum class Symbol{
    //@, r, !, :, .
    at = '@',
    r = '/',
    exclamation = '!',
    colon = ':',
    dot = '.'    
};

//на вход получаем вектор точек и вектор пылинок
void draw(const std::vector<Point_object>& points, const std::vector<Mote>& motes){
    // Определите границы вашего "холста"
    int width = MAX_WIDTH;
    int height = MAX_HEIGHT;

    
    std::vector<std::string> canvas(height, std::string(width, ' '));
    for (const auto& point : points) {
        Vector pos = point.getPosition();
        
        int x = static_cast<int>(pos.getX());
        int y = static_cast<int>(pos.getY());
        
        if (x >= 0 && x < width && y >= 0 && y < height) {
            canvas[y][x] = static_cast<char>(Symbol::at);
        }
    }

    for (const auto& mote : motes) {
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

    
    for (const auto& line : canvas) {
        std::cout << line << "\n";
    }
}

void draw2(const Segment& segment, const std::vector<Mote>& motes){
    // Определите границы вашего "холста"
    int width = MAX_WIDTH;
    int height = MAX_HEIGHT;

    
    std::vector<std::string> canvas(height, std::string(width, ' '));
    Vector start = segment.getPosition();
    Vector end{0, 0};
    if (segment.getVelocity().length()) end = start + segment.getVelocity().normalise() * segment.getLength();
    else end = start + Vector(1, 0) * segment.getLength();
    for (const auto& mote : motes) {
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

    Vector direction = end - start;
    for (int i = 0; i < direction.length(); i++){
        Vector pos = start + direction.normalise() * i;
        int x = static_cast<int>(pos.getX());
        int y = static_cast<int>(pos.getY());
        
        if (x >= 0 && x < width && y >= 0 && y < height) {
            canvas[y][x] = static_cast<char>(Symbol::at);
        }
    }
    
    for (const auto& line : canvas) {
        std::cout << line << "\n";
    }
}

int main() {
    std::vector<Point_object> points;
    points.push_back(Point_object({12, 12}, {1, 1}));

    Segment segment({12, 12}, 5, {1, 1});
    
    std::vector<Mote> motes;
    
    char key;
    while (true) {
        /*if (kbhit()) {
            key = getchar();
            if (key == 'w') {
                points[0].accelerate(Vector(0, -1));
            } else if (key == 's') {
                points[0].accelerate(Vector(0, 1));
            } else if (key == 'a') {
                points[0].accelerate(Vector(-1, 0));
            } else if (key == 'd') {
                points[0].accelerate(Vector(1, 0));
            }
            else if (key == 'q') {
                break;
            }
        }
        

        for (auto& point : points) {
            point.move(0.2);
        }
        
        draw(points, motes);
        
        for (auto& point : points) {
            auto newMotes = point.emit();
            motes.insert(motes.end(), newMotes.begin(), newMotes.end());
        }
        // Удаление пылинок
        motes.erase(std::remove_if(motes.begin(), motes.end(), [](Mote& mote) {return !mote.update(1);}), motes.end());
        
        usleep(10000);*/

        if (kbhit()) {
            key = getchar();
            if (key == 'w') {
                segment.accelerate(Vector(0, -1));
            } else if (key == 's') {
                segment.accelerate(Vector(0, 1));
            } else if (key == 'a') {
                segment.accelerate(Vector(-1, 0));
            } else if (key == 'd') {
                segment.accelerate(Vector(1, 0));
            }
            else if (key == 'q') {
                break;
            }
        }

        segment.move(0.2);

        draw2(segment, motes);

        
        auto newMotes = segment.emit();
        motes.insert(motes.end(), newMotes.begin(), newMotes.end());
        
        motes.erase(std::remove_if(motes.begin(), motes.end(), [](Mote& mote) {return !mote.update(1);}), motes.end());

        usleep(10000);
    }

    return 0;
}

void Point::move(double dt){
    
    if (position.getX() < 0){
        
        position.setX(-position.getX());
        velocity = velocity.reflect({1, 0});
    }
    if (position.getX() >= MAX_WIDTH){
        position.setX(2 * MAX_WIDTH - position.getX());
        velocity = velocity.reflect({-1, 0});
    }
    if (position.getY() < 0){
        position.setY(-position.getY());
        velocity = velocity.reflect({0, 1});
    }
    if (position.getY() >= MAX_HEIGHT){
        position.setY(2 * MAX_HEIGHT - position.getY());
        velocity = velocity.reflect({0, -1});
    }
    
    double aspectRatio = (24.0 / 11.0) * (static_cast<double>(MAX_HEIGHT) / MAX_WIDTH);
    Vector scaledVelocity = {velocity.getX() / aspectRatio, velocity.getY()};

    position = position + scaledVelocity * dt;
}

std::vector<Mote> Point_object::emit(){
    std::vector<Mote> newMotes;
    for (int i = 0; i < 11; i++){
        Vector moteVelocity = -getVelocity() - getVelocity().perpendicular() * 0.1 * (i-5);
        newMotes.push_back(Mote(getPosition(), moteVelocity));
    }
    return newMotes;
}

void Point_object::accelerate(const Vector& acceleration) {
    if ((getVelocity() + acceleration).length() < max_speed){
        setVelocity(getVelocity() + acceleration);
    }
}

bool Mote::update(double dt){
    double aspectRatio = (24.0 / 11.0) * (static_cast<double>(MAX_HEIGHT) / MAX_WIDTH);
    Vector scaledVelocity = {getVelocity().getX() / aspectRatio, getVelocity().getY()};

    setPosition(getPosition() + scaledVelocity * dt);
    lifeTime++;
    if (lifeTime < maxLifeTime){
        return true;
    }
    else return false;
}

void Segment::accelerate(const Vector& acceleration) {
    if ((getVelocity() + acceleration).length() < max_speed){
        setVelocity(getVelocity() + acceleration);
    }
}

std::vector<Mote> Segment::emit(){
    std::vector<Mote> newMotes;
    for (int i = 0; i < 11; i++){
        Vector moteVelocity = -getVelocity() - getVelocity().perpendicular() * 0.1 * (i-5);
        newMotes.push_back(Mote(getPosition(), moteVelocity));
    }
    return newMotes;
}

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab2
//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab2 && clang++ -std=c++17 main.cpp -o lab2.o && ./lab2.o
//clang++ -std=c++17 lab2/main.cpp -o lab2/lab2.o && ./lab2/lab2.o