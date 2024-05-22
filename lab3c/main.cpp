#include <GLUT/glut.h>
#include <vector>


#include "menu.h"


MainMenu main_menu;


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	main_menu.Draw(Point(0.0f, 0.0f));
	glutSwapBuffers();
}

void mouseButton(int button, int state, int x, int y) {
		main_menu.OnMouseClick(button, state, x, y);
		main_menu.OnMouseClick(button, state, x, y);
}

int main(int argc, char **argv) {
	
	MenuWindow menu_window;
	main_menu.ReadMenuFromFile("menu.txt");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	glutCreateWindow("Урок 1");
 
	glutDisplayFunc(renderScene);
	glutMouseFunc(mouseButton);
 
	glutMainLoop();
 
	return 1;
}

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab3c && clang++ -std=c++14 menu.cpp main.cpp -framework OpenGL -framework GLUT -o main && /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab3c/main