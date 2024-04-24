#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include <fstream>
#include <vector>
#include <string>

class Camera {
public:
    float angle = 0.0;
    float lx = 0.0f, lz = -1.0f;
    float x = 0.0f, z = 5.0f;
    float deltaAngle = 0.0f;
    float deltaMove = 0;
    int xOrigin = -1;

    void computePos(float deltaMove) {
        x += deltaMove * lx * 0.1f;
        z += deltaMove * lz * 0.1f;
    }
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

struct MenuItem {
    std::string name;
    int value;
};

struct Menu {
    std::string name;
    std::vector<MenuItem> items;
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
    Camera camera;
    Teapot teapot;
    MenuManager menuManager;

    Application() : menuManager(&teapot) {}

    void renderScene(void);
    int main(int argc, char **argv);
};

// Реализация методов классов

std::vector<Menu> MenuManager::readMenuFromFile(const std::string& filename) {
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

//считываем меню из файла
void MenuManager::createPopupMenus() {
    std::vector<Menu> menus = readMenuFromFile("menu.txt");
    mainMenu = glutCreateMenu(processMainMenu);
    for (const auto& menu : menus) {
        int subMenu = glutCreateMenu(processSubMenu);
        for (const auto& item : menu.items) {
            glutAddMenuEntry(item.name.c_str(), item.value);
        }
        glutAddSubMenu(menu.name.c_str(), subMenu);
    }
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Остальные методы классов должны быть реализованы аналогично

int main(int argc, char **argv) {
    Application app;
    return app.main(argc, argv);
}

void MenuManager::processMainMenu(int option) {
    switch (option) {
        case 1:
            fillMenu = glutCreateMenu(processFillMenu);
            glutAddMenuEntry("Fill", 1);
            glutAddMenuEntry("Wireframe", 2);
            glutAttachMenu(GLUT_RIGHT_BUTTON);
            break;
        case 2:
            shrinkMenu = glutCreateMenu(processShrinkMenu);
            glutAddMenuEntry("Increase", 1);
            glutAddMenuEntry("Decrease", 2);
            glutAttachMenu(GLUT_RIGHT_BUTTON);
            break;
        case 3:
            colorMenu = glutCreateMenu(processColorMenu);
            glutAddMenuEntry("Red", 1);
            glutAddMenuEntry("Green", 2);
            glutAddMenuEntry("Blue", 3);
            glutAttachMenu(GLUT_RIGHT_BUTTON);
            break;
        case 4:
            confirmWindow = glutCreateWindow("Confirm");
            glutDisplayFunc(confirmDisplay);
            glutKeyboardFunc(confirmKeyboard);
            glutMainLoop();
            break;
        case 5:
            glutDestroyWindow(mainWindow);
            exit(0);
            break;
        default:
            break;
    }
}

void MenuManager::processFillMenu(int option) {
    switch (option) {
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 2:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            break;
    }
}

void MenuManager::processShrinkMenu(int option) {
    switch (option) {
        case 1:
            teapot.scale = 0.5f;
            break;
        case 2:
            teapot.scale = 1.0f;
            break;
        default:
            break;
    }
}

void MenuManager::processColorMenu(int option) {
    switch (option) {
        case 1:
            teapot.red = 1.0f;
            teapot.green = 0.0f;
            teapot.blue = 0.0f;
            break;
        case 2:
            teapot.red = 0.0f;
            teapot.green = 1.0f;
            teapot.blue = 0.0f;
            break;
        case 3:
            teapot.red = 0.0f;
            teapot.green = 0.0f;
            teapot.blue = 1.0f;
            break;
        case 4:
            teapot.red = 1.0f;
            teapot.green = 0.5f;
            teapot.blue = 0.5f;
            break;
        default:
            break;
    }
}

void MenuManager::confirmDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(0.0, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Are you sure you want to exit?");
    glutSwapBuffers();
}

void MenuManager::confirmKeyboard(unsigned char key, int x, int y) {
    if (key == 'y') {
        confirm = true;
        glutDestroyWindow(confirmWindow);
        glutDestroyWindow(mainWindow);
        exit(0);
    } else if (key == 'n') {
        confirm = false;
        glutDestroyWindow(confirmWindow);
    }
}

void MenuManager::processSubMenu(int option) {
    switch (option) {
        case 1:
            teapot.red = 1.0f;
            teapot.green = 0.0f;
            teapot.blue = 0.0f;
            break;
        case 2:
            teapot.red = 0.0f;
            teapot.green = 1.0f;
            teapot.blue = 0.0f;
            break;
        case 3:
            teapot.red = 0.0f;
            teapot.green = 0.0f;
            teapot.blue = 1.0f;
            break;
        case 4:
            glutDestroyWindow(mainWindow);
            exit(0);
            break;
        default:
            break;
    }
}

void Application::renderScene(void) {
    if (menuManager.menuFlag == 0) {
        if (camera.deltaMove) {
            camera.computePos(camera.deltaMove);
        }
        if (camera.deltaAngle) {
            camera.angle += camera.deltaAngle;
            camera.lx = sin(camera.angle);
            camera.lz = -cos(camera.angle);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(camera.x, 1.0f, camera.z,
                  camera.x + camera.lx, 1.0f, camera.z + camera.lz,
                  0.0f, 1.0f, 0.0f);

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glScalef(teapot.scale, teapot.scale, teapot.scale);
        teapot.draw();
        glPopMatrix();

        glutSwapBuffers();
    }
}

void processNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) {
        glutDestroyWindow(mainWindow);
        exit(0);
    }
}

void pressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camera.deltaMove = 0.5f;
            break;
        case GLUT_KEY_DOWN:
            camera.deltaMove = -0.5f;
            break;
        case GLUT_KEY_LEFT:
            camera.deltaAngle = -0.01f;
            break;
        case GLUT_KEY_RIGHT:
            camera.deltaAngle = 0.01f;
            break;
    }
}

void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            camera.deltaMove = 0;
            break;
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            camera.deltaAngle = 0;
            break;
    }
}

void mouseMove(int x, int y) {
    if (camera.xOrigin >= 0) {
        camera.deltaAngle = (x - camera.xOrigin) * 0.001f;
    }
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            camera.angle += camera.deltaAngle;
            camera.xOrigin = -1;
        } else {
            camera.xOrigin = x;
        }
    }
}


int Application::main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    mainWindow = glutCreateWindow("OpenGL Lab 3.2");

    glutDisplayFunc(renderScene);
    glutReshapeFunc([](int w, int h) {
        if (h == 0) h = 1;
        float ratio = 1.0 * w / h;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, w, h);
        gluPerspective(45, ratio, 0.1, 100);
        glMatrixMode(GL_MODELVIEW);
    });

    glutIdleFunc(renderScene);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    menuManager.createPopupMenus();
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}

int main(int argc, char **argv) {
    Application app;
    return app.main(argc, argv);
}

