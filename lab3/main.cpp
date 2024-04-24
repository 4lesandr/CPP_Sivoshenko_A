#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include <fstream>


int mainMenu;

float red = 1.0f, blue=0.5f, green=0.5f;


int menuFlag = 0;
int confirmWindow;
int mainWindow;

bool confirm = false;

struct MenuItem {
    std::string name;
    int value;
};

struct Menu {
    std::string name;
    std::vector<MenuItem> items;
};

class Teapot {
public:
    float red = 1.0f, blue = 0.5f, green = 0.5f;
    float scale = 1.0f;

    void draw() {
        glColor3f(red, green, blue);
        glutSolidTeapot(0.5);
    }
};

class MenuManager {
public:
    int fillMenu, shrinkMenu, mainMenu, colorMenu;
    int menuFlag = 0;
    int confirmWindow;
    bool confirm = false;
    float prevRed = 1.0f, prevGreen = 0.5f, prevBlue = 0.5f;
    Teapot* teapot;

    MenuManager(Teapot* teapot) : teapot(teapot) {}

    std::vector<Menu> readMenuFromFile(const std::string& filename);
    void createPopupMenus();
    void processMenuStatus(int status, int x, int y);
    void processMainMenu(int option);
    void processFillMenu(int option);
    void processShrinkMenu(int option);
    void processColorMenu(int option);
    void confirmDisplay();
    void confirmKeyboard(unsigned char key, int x, int y);
    void processSubMenu(int option);
};

class Application {
public:
    int mainWindow;

    Teapot teapot;
    MenuManager menuManager;

    Application() : menuManager(&teapot) {}

    void renderScene(void);
    int main(int argc, char **argv);
};
 
void drawTeapot() {
    glColor3f(red, green, blue);
    glutSolidTeapot(0.5);
}
 
void renderScene(void) {
    if (glutGetWindow() != mainWindow) {
        return;
    }

    // Очистка буфера цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Сброс просмотра
    glLoadIdentity();
    //чайник
    //цвет
    glColor3f(red, green, blue);
    
    drawTeapot();
    
    
    glutSwapBuffers();
}
// ------------------------------------//
//            меню  					//
// ------------------------------------//
 


void processMenuStatus(int status, int x, int y) {
	if (status == GLUT_MENU_IN_USE)
		menuFlag = 1;
	else
		menuFlag = 0;
}
 
void processMainMenu(int option) {
	//ничего здесь не делаем
	//все действия для подменю
}

void processColorMenu(int option) {
	switch (option) {
		case 1 :
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f; break;
		case 2 :
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f; break;
		case 3 :
			red = 0.0f;
			green = 0.0f;
			blue = 1.0f; break;
		case 4 :
			red = 1.0f;
			green = 0.5f;
			blue = 0.5f; break;
	}
}

std::vector<Menu> readMenuFromFile(const std::string& filename) {
    std::vector<Menu> menus;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Menu menu;
        menu.name = line;
        while (std::getline(file, line) && !line.empty()) {
            MenuItem item;
            size_t pos = line.find(' ');
            if (pos != std::string::npos) {
                item.name = line.substr(0, pos);
                item.value = std::stoi(line.substr(pos + 1));
            }
            menu.items.push_back(item);
        }
        menus.push_back(menu);
    }
    return menus;
}



void confirmDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    // Установка цвета прямоугольника
    glColor3f(0.5f, 0.5f, 0.5f);

    // Рисование прямоугольника
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();

    // Установка цвета текста
    glColor3f(1.0f, 1.0f, 1.0f);

    
    

    // Отображение текста
    std::string text = "Are you sure? (y/n)";
	//выравнивание текста по середине квадрата в зависимости от длины строки
	glRasterPos2f(0.5f - text.size() * 0.05, 0.0f);

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutSwapBuffers();
}

void confirmKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'y':
        case 'Y':
            // Пользователь подтвердил действие
			confirm = true;
            glutDestroyWindow(confirmWindow);
            break;
        case 'n':
        case 'N':
            // Пользователь отменил действие
			confirm = false;
            glutDestroyWindow(confirmWindow);
            break;
    }
}

void processSubMenu(int option) {
    // Создание нового окна для подтверждения
    glutInitWindowPosition(150,150);
    glutInitWindowSize(400,200);
    confirmWindow = glutCreateWindow("Подтвердите выбор");
    glutDisplayFunc(confirmDisplay);
    glutKeyboardFunc(confirmKeyboard);  // Добавление обработчика клавиатуры
	if (!confirm) option = 5;
    // Здесь вы можете зарегистрировать другие функции обратного вызова для нового окна
    glutPostRedisplay();
    // Обработка выбора пункта подменю
    switch (option) {
        case 1:
            // Действие для первого пункта подменю
            red = 1.0f;
            green = 0.0f;
            blue = 0.0f;
            break;
        case 2:
            // Действие для второго пункта подменю
            red = 0.0f;
            green = 1.0f;
            blue = 0.0f;
            break;
        case 3:
            red = 0.0f;
            green = 0.0f;
            blue = 1.0f;
            break;
        case 4:
			//завершение программы
			glutDestroyWindow(mainWindow);
			exit(0);
            break;
        default:
            break;
    }
    // Установка функции обратного вызова для главного окна
    glutSetWindow(mainWindow);
    glutDisplayFunc(renderScene);
}
 
void createPopupMenus() {
    std::vector<Menu> menus = readMenuFromFile("menu.txt");

    // Создание главного меню
    int mainMenu = glutCreateMenu(processMainMenu);

    // Добавление пунктов меню из файла
    for (const auto& menu : menus) {
        // Создание подменю
        int submenu = glutCreateMenu(processSubMenu);

        // Добавление пунктов подменю
        for (const auto& item : menu.items) {
            glutAddMenuEntry(item.name.c_str(), item.value);
        }

        // Добавление подменю в главное меню
        glutSetMenu(mainMenu);
        glutAddSubMenu(menu.name.c_str(), submenu);
    }

    // Привязка главного меню к правой кнопке мыши
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMenuStatusFunc(processMenuStatus);
}
 
// ------------------------------------	//
//             main()					//
// -----------------------------------	//
 
int main(int argc, char **argv) {
 
	// инициализация GLUT и создание окна
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	mainWindow = glutCreateWindow("OpenGL Blblla");
 
	//регистрация
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
     
	// OpenGL инициализация функций
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
 
	// инициализация меню
	createPopupMenus();
 
	// Главный цикл
	glutMainLoop();
 
	return 1;
}

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab3 && clang++ -std=c++17 main.cpp -framework OpenGL -framework GLUT -o main && /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab3/main