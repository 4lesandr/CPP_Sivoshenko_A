#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include <fstream>

 

int fillMenu, shrinkMenu, mainMenu, colorMenu;

float red = 1.0f, blue=0.5f, green=0.5f;

float scale = 1.0f;

int menuFlag = 0;
int confirmWindow;
int mainWindow;
float prevRed = 1.0f, prevGreen = 0.5f, prevBlue = 0.5f;
bool confirm = false;

struct MenuItem {
    std::string name;
    int value;
};

struct Menu {
    std::string name;
    std::vector<MenuItem> items;
};

 
void drawTeapot() {
    glColor3f(red, green, blue);
    glutSolidTeapot(0.5);
}
 
 
void renderScene(void) {
    if (glutGetWindow() != mainWindow) {
        return;
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();


    glColor3f(red, green, blue);
    
    drawTeapot();
    
    
    glutSwapBuffers();
}



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
 
void processFillMenu(int option) {
	switch (option) {
		case 1: glPolygonMode(GL_FRONT, GL_FILL); break;
		case 2: glPolygonMode(GL_FRONT, GL_LINE); break;
	}
}
 
void processShrinkMenu(int option) {
	switch (option) {
		case 1: scale = 0.5f; break;
		case 2: scale = 1.0f; break;
	}
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
	
    
    glColor3f(0.5f, 0.5f, 0.5f);

    
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();

    
    glColor3f(1.0f, 1.0f, 1.0f);

    
    


    std::string text = "Are you sure? (y/n)";

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
    
			confirm = true;
            glutDestroyWindow(confirmWindow);
            break;
        case 'n':
        case 'N':
    
			confirm = false;
            glutDestroyWindow(confirmWindow);
            break;
    }
}

void processSubMenu(int option) {
 
    glutInitWindowPosition(150,150);
    glutInitWindowSize(400,200);
    
    confirmWindow = glutCreateWindow("Подтвердите выбор");
    
    glutDisplayFunc(confirmDisplay);
    glutKeyboardFunc(confirmKeyboard);
    glutPostRedisplay();

	if (!confirm) option = 5;
        
    switch (option) {
        case 1:
    
            red = 1.0f;
            green = 0.0f;
            blue = 0.0f;
            break;
        case 2:
    
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
	
			glutDestroyWindow(mainWindow);
			exit(0);
            break;
        default:
            break;
    }
    
    glutSetWindow(mainWindow);
    glutDisplayFunc(renderScene);
}
 
void createPopupMenus() {
    std::vector<Menu> menus = readMenuFromFile("menu.txt");

    
    int mainMenu = glutCreateMenu(processMainMenu);

    
    for (const auto& menu : menus) {

        int submenu = glutCreateMenu(processSubMenu);


        for (const auto& item : menu.items) {
            glutAddMenuEntry(item.name.c_str(), item.value);
        }


        glutSetMenu(mainMenu);
        glutAddSubMenu(menu.name.c_str(), submenu);
    }


    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMenuStatusFunc(processMenuStatus);
}
 

 
int main(int argc, char **argv) {
 

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	mainWindow = glutCreateWindow("OpenGL Blblla");
 

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
     

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
 

	createPopupMenus();
 

	glutMainLoop();
 
	return 1;
}

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab3 && clang++ -std=c++17 main.cpp -framework OpenGL -framework GLUT -o main && /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab3/main