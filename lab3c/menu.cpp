#include "menu.h"

MenuItem::MenuItem() {
    color = Color();
    position = Point();
    size = Size();
}

void Button::Draw(Point reference_point) const {
    if (active) {
        //уставливаем цвет кнопки
        glColor3f(color.r, color.g, color.b);
        if (pressed) {
            glColor3f(highlightColor.r, highlightColor.g, highlightColor.b);
        }
        glBegin(GL_QUADS);
        glVertex2f(position.x + reference_point.x, position.y + reference_point.y);
        glVertex2f(position.x + size.width + reference_point.x, position.y + reference_point.y);
        glVertex2f(position.x + size.width + reference_point.x, position.y + size.height + reference_point.y);
        glVertex2f(position.x + reference_point.x, position.y + size.height + reference_point.y);
        glEnd();
        //текст кнопки
        glColor3f(0.0f, 0.0f, 0.0f);
        //позиция от -0.25 до 0.25 по x и от -0.25 до 0.0 по y
        glRasterPos2f(position.x + reference_point.x + 0.1f, position.y + reference_point.y + 0.1f);
        for (int i = 0; i < name.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[i]);
        }
    }
	
    printf("Button::Draw %s, active: %d\n", name.c_str(), active);
    
    printf("Button::Draw\n");
}

void Dialog::Draw(Point reference_point) const{
    if (active) {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        glVertex2f(position.x + reference_point.x, position.y + reference_point.y);
        glVertex2f(position.x + size.width + reference_point.x, position.y + reference_point.y);
        glVertex2f(position.x + size.width + reference_point.x, position.y + size.height + reference_point.y);
        glVertex2f(position.x + reference_point.x, position.y + size.height + reference_point.y);
        glEnd();
        for (auto& button : buttons) {
            button.Draw(reference_point);
        }
        //текст диалога
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(position.x + reference_point.x + 0.90f, position.y + reference_point.y + 0.75f);
        for (int i = 0; i < title.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
        }
    }
}

void MenuWindow::Draw(Point reference_point) const {
    for (auto& Button : buttons) {
        Button.Draw(reference_point);
    }
    for (auto& Dialog : dialogs) {
        Dialog.Draw(reference_point);
    }
}

void MenuWindow::ButtonsAction(Point mouse_position) {
    for (auto& Button : buttons) {
        Button.setPressed(false);
        // printf("Button %s unpressed\n", Button.getName().c_str());
        // printf("Button position: %f %f %f %f\n", Button.getPosition().x, Button.getPosition().y, Button.getSize().width + Button.getPosition().x, Button.getSize().height + Button.getPosition().y);
        if (Button.getPosition().x <= mouse_position.x && Button.getPosition().y <= mouse_position.y && Button.getSize().width + Button.getPosition().x >= mouse_position.x && Button.getSize().height + Button.getPosition().y >= mouse_position.y) {
            Button.setPressed(true);
            // printf("Button %s pressed\n", Button.getName().c_str());
            if (Button.getName() == "Exit") {
                for (auto& Dialog : dialogs) {
                    if (Dialog.getTitle() == "Quit?") {
                        Dialog.setActive(true);
                        printf("Dialog1 active\n");
                    }
                }
                for (auto& Button_sub : buttons) {
                    if (Button_sub.getName() == "Button5" || Button_sub.getName() == "Button6") {
                        Button_sub.setActive(false);
                        printf("Button %s inactive\n", Button_sub.getName().c_str());
                    }
                }
            }
            if (Button.getName() == "Button2") {
                for (auto& Dialog : dialogs) {
                    if (Dialog.getTitle() == "Quit?") {
                        Dialog.setActive(false);
                        printf("Dialog1 inactive\n");
                    }
                }
                for (auto& Button_sub : buttons) {
                    if (Button_sub.getName() == "Button5" || Button_sub.getName() == "Button6") {
                        Button_sub.setActive(true);
                        printf("Button %s active\n", Button_sub.getName().c_str());
                        printf("Button active? %d\n", Button_sub.getActive());
                    }
                }
            }
        }
    }
}

void MenuWindow::DialogsAction(Point mouse_position) {
    for (auto& Dialog : dialogs) {
        for (auto& Button : Dialog.getButtons()) {
            Button.setPressed(false);
            if (Button.getPosition().x <= mouse_position.x && Button.getPosition().y <= mouse_position.y && Button.getSize().width + Button.getPosition().x >= mouse_position.x && Button.getSize().height + Button.getPosition().y >= mouse_position.y) {
                Button.setPressed(true);
                if (Button.getName() == "Yes") {
                    printf("Yes pressed\n");
                    exit(0);
                }
                if (Button.getName() == "No") {
                    printf("No pressed\n");
                    for (auto& Dialog : dialogs) {
                        if (Dialog.getTitle() == "Quit?") {
                            Dialog.setActive(false);
                            printf("Dialog1 inactive\n");
                        }
                    }
                }
            }
        }
    }
}

void MainMenu::Draw(Point reference_point) const {
    menu_window.Draw(reference_point);
}

void MainMenu::addWindow(MenuWindow window) {
    menu_window = window;
}

void MainMenu::OnMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //перевод в координаты OpenGL
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        float normalizedX = -1.0f + 2 * static_cast<float>(x) / windowWidth;
        float normalizedY = 1.0f - 2 * static_cast<float>(y) / windowHeight;
        printf("Mouse pressed at %f %f\n", normalizedX, normalizedY);

        Point mouse_position(normalizedX, normalizedY);
        menu_window.ButtonsAction(mouse_position);
        menu_window.DialogsAction(mouse_position);
        glutPostRedisplay();
    }
}

void MainMenu::ReadMenuFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Can't open file " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "Dialog") {
            float r, g, b;
            float x, y, width, height;
            std::string title;
            iss >> r >> g >> b >> x >> y >> width >> height >> title;
            Dialog dialog(Color(r, g, b), Point(x, y), Size(width, height), title);
            while (std::getline(file, line) && line != "}") {
                std::istringstream iss(line);
                std::string type;
                iss >> type;
                if (type == "Button") {
                    float r, g, b;
                    float x, y, width, height;
                    int active;
                    std::string name;
                    iss >> r >> g >> b >> x >> y >> width >> height >> active >> name;
                    Button button(Color(r, g, b), Point(x, y), Size(width, height), active, name);
                    dialog.AddButton(button);
                }
            }
            menu_window.AddDialog(dialog);
        } else if (type == "Button") {
            float r, g, b;
            float x, y, width, height;
            int active;
            std::string name;
            iss >> r >> g >> b >> x >> y >> width >> height >> active >> name;
            Button button(Color(r, g, b), Point(x, y), Size(width, height), active, name);
            menu_window.AddButton(button);
        }
    }
    file.close();
}
