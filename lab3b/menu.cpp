#include "menu.h"

// ������� ��� ������ ����� ��������
string ReadName(const string& line) {
    size_t pos = line.find("=");
    if (pos != string::npos) {
        return line.substr(pos + 1);
    }
    return "";
}

// Функция для чтения позиции (x, y)
void ReadPosition(const std::string& line, GLfloat position[2]) {
    size_t openBracketPos = line.find("(");
    size_t commaPos = line.find(",");
    size_t closeBracketPos = line.find(")");

    if (openBracketPos != std::string::npos && commaPos != std::string::npos && closeBracketPos != std::string::npos) {
        std::string xStr = line.substr(openBracketPos + 1, commaPos - openBracketPos - 1);
        std::string yStr = line.substr(commaPos + 1, closeBracketPos - commaPos - 1);
        position[0] = std::stof(xStr);
        position[1] = std::stof(yStr);
    }
}

// Функция для чтения размера (width, height)
void ReadSize(const std::string& line, GLfloat size[2]) {
    size_t openBracketPos = line.find("(");
    size_t semicolonPos = line.find(",");
    size_t closeBracketPos = line.find(")");

    if (openBracketPos != std::string::npos && semicolonPos != std::string::npos && closeBracketPos != std::string::npos) {
        std::string widthStr = line.substr(openBracketPos + 1, semicolonPos - openBracketPos - 1);
        std::string heightStr = line.substr(semicolonPos + 1, closeBracketPos - semicolonPos - 1);
        size[0] = std::stof(widthStr);
        size[1] = std::stof(heightStr);
    }
}

// Функция для чтения цвета (r, g, b)
void ReadColor(const std::string& line, GLfloat color[3]) {
    size_t openBracketPos = line.find("(");
    size_t comma1Pos = line.find(",", openBracketPos + 1);
    size_t comma2Pos = line.find(",", comma1Pos + 1);
    size_t closeBracketPos = line.find(")", comma2Pos + 1);

    if (openBracketPos != std::string::npos && comma1Pos != std::string::npos && comma2Pos != std::string::npos && closeBracketPos != std::string::npos) {
        std::string rStr = line.substr(openBracketPos + 1, comma1Pos - openBracketPos - 1);
        std::string gStr = line.substr(comma1Pos + 1, comma2Pos - comma1Pos - 1);
        std::string bStr = line.substr(comma2Pos + 1, closeBracketPos - comma2Pos - 1);

        // Убедитесь, что значения не пусты
        if (!rStr.empty() && !gStr.empty() && !bStr.empty()) {
            color[0] = std::stof(rStr) / 255.0f;  // Деление на 255 для преобразования в диапазон [0, 1]
            color[1] = std::stof(gStr) / 255.0f;
            color[2] = std::stof(bStr) / 255.0f;
        }
    }
}


void MainMenu::ReadMenuFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    MenuWindow currentWindow;
    while (std::getline(file, line)) {
        if (line.find("MenuBegin") != std::string::npos) {
        }
        else if (line.find("WindowBegin") != std::string::npos) {
            GLfloat color[3];
            GLfloat position[2];
            GLfloat size[2];

            std::getline(file, line);
            ReadColor(line, color);
            std::getline(file, line);
            ReadPosition(line, position);
            std::getline(file, line);
            ReadSize(line, size);

            currentWindow = MenuWindow(color, position, size);
        }
        else if (line.find("ButtonBegin") != std::string::npos) {
            std::string name;
            GLfloat color[3], highlightColor[3];
            GLfloat position[2];
            GLfloat size[2];

            std::getline(file, line);
            name = ReadName(line);
            std::getline(file, line);
            ReadPosition(line, position);
            std::getline(file, line);
            ReadSize(line, size);
            std::getline(file, line);
            ReadColor(line, color);
            std::getline(file, line);
            ReadColor(line, highlightColor);

            currentWindow.addButton(Button(name, color, position, size, highlightColor));
        }
        else if (line.find("DialogBegin") != std::string::npos) {
            std::string name;
            GLfloat color[3];
            GLfloat position[2];
            GLfloat size[2];

            std::getline(file, line);
            name = ReadName(line);
            std::getline(file, line);
            ReadPosition(line, position);
            std::getline(file, line);
            ReadSize(line, size);
            std::getline(file, line);
            ReadColor(line, color);

            Dialog dialog(name, color, position, size);

            while (std::getline(file, line)) {
                if (line.find("ButtonBegin") != std::string::npos) {
                    std::string buttonName;
                    GLfloat buttonColor[3], buttonHighlightColor[3];
                    GLfloat buttonPosition[2];
                    GLfloat buttonSize[2];

                    std::getline(file, line);
                    buttonName = ReadName(line);
                    std::getline(file, line);
                    ReadPosition(line, buttonPosition);
                    std::getline(file, line);
                    ReadSize(line, buttonSize);
                    std::getline(file, line);
                    ReadColor(line, buttonColor);
                    std::getline(file, line);
                    ReadColor(line, buttonHighlightColor);

                    dialog.addButton(Button(buttonName, buttonColor, buttonPosition, buttonSize, buttonHighlightColor));
                }
                else if (line.find("DialogEnd") != std::string::npos) {
                    break;
                }
            }

            currentWindow.addDialog(dialog);
        }
        else if (line.find("WindowEnd") != std::string::npos) {
            addWindow(currentWindow);
        }
    }

    file.close();
}

void MainMenu::OnMouseClick(GLfloat point[2]) {
    for (auto& window : windows) {
        window.ButtonsAction(point);
        window.DialogsAction(point);
    }
}

void MainMenu::Draw() const {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Установка цвета фона в белый
    glClear(GL_COLOR_BUFFER_BIT); // Очистка буфера цвета

    for (const auto& window : windows) {
        window.Draw();
    }

    glutSwapBuffers(); // Обмен буферов для двойной буферизации
}

void MainMenu::addWindow(const MenuWindow& window) {
    windows.push_back(window);
}

void MenuWindow::DialogsAction(GLfloat point[2]) {
    GLfloat new_pos[2];

    for (auto& dialog : dialogs) {
        for (auto& button : dialog.getButtons()) {

            new_pos[0] = button.getPosition()[0] + dialog.getPosition()[0] + position[0];
            new_pos[1] = button.getPosition()[1] + dialog.getPosition()[1] + position[1];

            if (point[0] >= new_pos[0] && point[0] <= new_pos[0] + button.getSize()[0] &&
                point[1] >= new_pos[1] && point[1] <= new_pos[1] + button.getSize()[1]) {
                button.ToggleState(); // Изменение состояния кнопки

                if (button.IsPressed()) {
                    if (button.getName() == "Sad") {
                        std::cout << "Important message: Drink a beer" << std::endl;
                    }
                    else if (button.getName() == "Funny") {
                        std::cout << "Important message: Drink a beer" << std::endl;
                    }
                }

                break;
            }
        }
    }
}

void MenuWindow::DialogsAction(int x, int y) {
    for (auto& dialog : dialogs) {
        for (auto& button : dialog.getButtons()) {
            int new_x = button.getPosition().x + dialog.getPosition().x + position.x;
            int new_y = button.getPosition().y + dialog.getPosition().y + position.y;

            if (x >= new_x && x <= new_x + button.getSize().cx &&
                y >= new_y && y <= new_y + button.getSize().cy) {
                button.ToggleState(); // Изменяем состояние кнопки

                if (button.IsPressed()) {
                    if (button.getName() == "Sad" || button.getName() == "Funny") {
                        // Выводим сообщение в окне
                        printf("Drink a beer\n");
                    }
                }

                break;
            }
        }
    }
}

void MenuWindow::ButtonsAction(int x, int y) {
    for (auto& button : buttons) {
        int new_x = button.getPosition().x + position.x;
        int new_y = button.getPosition().y + position.y;

        if (x >= new_x && x <= new_x + button.getSize().cx &&
            y >= new_y && y <= new_y + button.getSize().cy) {

            if (button.getName() == "Button1") {
                for (auto& dialog : dialogs) {
                    if (dialog.getActive() == false) button.ToggleState();

                    if (dialog.getName() == "How you feeling?") {
                        if (!dialog.getActive()) dialog.setActive(!dialog.getActive());
                        else {
                            for (auto& button1 : dialog.getButtons()) {
                                if (button1.IsPressed()) {
                                    dialog.setActive(!dialog.getActive());
                                    button.ToggleState();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else if (button.getName() == "Button2") {
                button.ToggleState();

                if (button.IsPressed()) {
                    // Выводим сообщение с опцией закрытия меню
                    printf("Do you want to close a menu?\n");
                }
            }
            else {
                button.ToggleState(); // Изменяем состояние кнопки
            }
            break;
        }
    }
}


void MenuWindow::Draw() const {
    // Установка цвета и позиции для рисования окна меню
    glColor3fv(color);
    glRectf(position[0], position[1], position[0] + size[0], position[1] + size[1]);

    // Отрисовка кнопок внутри окна меню
    for (const auto& button : buttons) {
        button.Draw();
    }

    // Отрисовка диалогов внутри окна меню
    for (const auto& dialog : dialogs) {
        dialog.Draw();
    }
}

std::vector<Dialog>& MenuWindow::getDialogs() {
    return dialogs;
}

std::vector<Button>& MenuWindow::getButtons() {
    return buttons;
}

void MenuWindow::setColor(GLfloat _color[3]) {
    for (int i = 0; i < 3; i++) color[i] = _color[i];
}

void MenuWindow::getColor(GLfloat _color[3]) const {
    for (int i = 0; i < 3; i++) _color[i] = color[i];
}

void MenuWindow::setPosition(GLfloat _position[2]) {
    for (int i = 0; i < 2; i++) position[i] = _position[i];
}

void MenuWindow::getPosition(GLfloat _position[2]) const {
    for (int i = 0; i < 2; i++) _position[i] = position[i];
}

void MenuWindow::setSize(GLfloat _size[2]) {
    for (int i = 0; i < 2; i++) size[i] = _size[i];
}

void MenuWindow::getSize(GLfloat _size[2]) const {
    for (int i = 0; i < 2; i++) _size[i] = size[i];
}

void MenuWindow::addButton(const Button& button) {
    buttons.push_back(button);
}

void MenuWindow::addDialog(const Dialog& dialog) {
    dialogs.push_back(dialog);
}

void Dialog::Draw() const {
    if (active) {
        // Установка цвета и позиции для рисования диалога
        glColor3fv(color);
        glRectf(position[0], position[1], position[0] + size[0], position[1] + size[1]);

        // Рисование кнопок внутри диалога
        for (const auto& button : buttons) {
            button.Draw();
        }
    }
}


void Dialog::setName(string _name) { name = _name; }
string Dialog::getName() const { return name; }
bool Dialog::getActive() const { return active; }
void Dialog::setActive(bool _active) { active = _active; }

void Dialog::addButton(const Button& button) {
    buttons.push_back(button);
}

vector<Button>& Dialog::getButtons() {
    return buttons;
}

void Dialog::ToggleState() {

    if (!active) active = !active;
    else {

        for (auto& button : buttons) {

            if (button.IsPressed()) { active = !active; break; }
        }

    }
}

void Button::Draw() const {
    // Установка цвета и позиции для рисования кнопки
    if (!pressed) {
        glColor3fv(color);
    } else {
        glColor3fv(highlightColor);
    }

    glRectf(position[0], position[1], position[0] + size[0], position[1] + size[1]);

    // Отрисовка текста кнопки
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(position[0] + size[0] / 2 - 0.05f, position[1] + size[1] / 2 - 0.05f);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}
void Button::setName(std::__1::string _name) { name = _name; }
std::string Button::getName() const { return name; }

void Button::setHighlightColor(GLfloat _highlightColor[3]) {
    for (int i = 0; i < 3; i++) highlightColor[i] = _highlightColor[i];
}

void Button::getHighlightColor(GLfloat _highlightColor[3]) const {
    for (int i = 0; i < 3; i++) _highlightColor[i] = highlightColor[i];
}

void Button::ToggleState() { pressed = !pressed; }
void Button::SetPressed(bool isPressed) { pressed = isPressed; }
bool Button::IsPressed() const { return pressed; }

void MenuItem::setColor(GLfloat _color[3]) {
    for (int i = 0; i < 3; i++) color[i] = _color[i];
}

void MenuItem::getColor(GLfloat _color[3]) const {
    for (int i = 0; i < 3; i++) _color[i] = color[i];
}

void MenuItem::setPosition(GLfloat _position[2]) {
    for (int i = 0; i < 2; i++) position[i] = _position[i];
}

void MenuItem::getPosition(GLfloat _position[2]) const {
    for (int i = 0; i < 2; i++) _position[i] = position[i];
}

void MenuItem::setSize(GLfloat _size[2]) {
    for (int i = 0; i < 2; i++) size[i] = _size[i];
}

void MenuItem::getSize(GLfloat _size[2]) const {
    for (int i = 0; i < 2; i++) _size[i] = size[i];
}