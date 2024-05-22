#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <GLUT/glut.h>

using namespace std;

// ������� ����� ��� ��������� ����
class MenuItem {
protected:
    GLfloat color[3]; 
    GLfloat position[2];
    GLfloat size[2];

public:
    MenuItem(GLfloat _color[3], GLfloat _position[2], GLfloat _size[2]) {
        for (int i = 0; i < 3; i++) color[i] = _color[i];
        for (int i = 0; i < 2; i++) position[i] = _position[i];
        for (int i = 0; i < 2; i++) size[i] = _size[i];
    }
    MenuItem() {
        color[0] = color[1] = color[2] = 0.0f;  // Black color
        position[0] = position[1] = 0.0f;  // Position at origin
        size[0] = size[1] = 0.0f;  // Size zero
    }
    virtual ~MenuItem() {}

    // Method for drawing the item (abstract)
    virtual void Draw() const = 0;

    // Setters and getters for color, position and size
    void setColor(GLfloat _color[3]) {
        for (int i = 0; i < 3; i++) color[i] = _color[i];
    }
    void getColor(GLfloat _color[3]) const {
        for (int i = 0; i < 3; i++) _color[i] = color[i];
    }

    void setPosition(GLfloat _position[2]) {
        for (int i = 0; i < 2; i++) position[i] = _position[i];
    }
    void getPosition(GLfloat _position[2]) const {
        for (int i = 0; i < 2; i++) _position[i] = position[i];
    }

    void setSize(GLfloat _size[2]) {
        for (int i = 0; i < 2; i++) size[i] = _size[i];
    }
    void getSize(GLfloat _size[2]) const {
        for (int i = 0; i < 2; i++) _size[i] = size[i];
    }
};

// ����� ��� ������
class Button : public MenuItem {
private:
    std::string name;
    GLfloat highlightColor[3];
    bool pressed;

public:
    Button(std::string _name, GLfloat _color[3], GLfloat _position[2], GLfloat _size[2], GLfloat _highlightColor[3])
        : MenuItem(_color, _position, _size), name(_name), pressed(false) {
            for (int i = 0; i < 3; i++) highlightColor[i] = _highlightColor[i];
        }
    Button() : MenuItem(), name(""), pressed(false) {
        highlightColor[0] = highlightColor[1] = highlightColor[2] = 0.0f;  // Black color
    }
    ~Button() {}

    // Method for drawing the button
    virtual void Draw() const override {
        // Implementation depends on how you want to draw the button
    }

    // Setters and getters for name, highlightColor and pressed
    void setName(std::string _name) ;
    std::string getName() const {
        return name;
    }

    void setHighlightColor(GLfloat _highlightColor[3]) {
        for (int i = 0; i < 3; i++) highlightColor[i] = _highlightColor[i];
    }
    void getHighlightColor(GLfloat _highlightColor[3]) const {
        for (int i = 0; i < 3; i++) _highlightColor[i] = highlightColor[i];
    }

    void ToggleState() {
        pressed = !pressed;
    }
    void SetPressed(bool isPressed) {
        pressed = isPressed;
    }

    bool IsPressed() const {
        return pressed;
    }
};

// ����� ��� ���������� ����
class Dialog : public MenuItem {
private:
    std::string name;
    std::vector<Button> buttons;
    bool active;

public:
    Dialog(std::string _name, GLfloat _color[3], GLfloat _position[2], GLfloat _size[2])
        : MenuItem(_color, _position, _size), name(_name), active(false) {}
    Dialog() : MenuItem(), name(""), active(false) {}
    ~Dialog() {}

    // Method for drawing the dialog
    virtual void Draw() const override {
        // Implementation depends on how you want to draw the dialog
    }

    // Setters and getters for name, active and buttons
    void setName(std::string _name) {
        name = _name;
    }
    std::string getName() const {
        return name;
    }

    bool getActive() const {
        return active;
    }
    void setActive(bool _active) {
        active = _active;
    }

    void addButton(const Button& button) {
        buttons.push_back(button);
    }

    std::vector<Button>& getButtons() {
        return buttons;
    }

    void ToggleState() {
        active = !active;
    }
};
class MenuWindow : public MenuItem {
private:
    std::vector<Button> buttons;
    std::vector<Dialog> dialogs;

public:
    MenuWindow(GLfloat _color[3], GLfloat _position[2], GLfloat _size[2])
        : MenuItem(_color, _position, _size) {}
    MenuWindow() : MenuItem() {}

    // Setters and getters for color, position and size
    void setColor(GLfloat _color[3]) {
        for (int i = 0; i < 3; i++) color[i] = _color[i];
    }
    void getColor(GLfloat _color[3]) const {
        for (int i = 0; i < 3; i++) _color[i] = color[i];
    }

    void setPosition(GLfloat _position[2]) {
        for (int i = 0; i < 2; i++) position[i] = _position[i];
    }
    void getPosition(GLfloat _position[2]) const {
        for (int i = 0; i < 2; i++) _position[i] = position[i];
    }

    void setSize(GLfloat _size[2]) {
        for (int i = 0; i < 2; i++) size[i] = _size[i];
    }
    void getSize(GLfloat _size[2]) const {
        for (int i = 0; i < 2; i++) _size[i] = size[i];
    }

    void addButton(const Button& button) {
        buttons.push_back(button);
    }

    void addDialog(const Dialog& dialog) {
        dialogs.push_back(dialog);
    }

    std::vector<Button>& getButtons() {
        return buttons;
    }

    std::vector<Dialog>& getDialogs() {
        return dialogs;
    }

    // Method for drawing the menu window
    virtual void Draw() const override {
        // Implementation depends on how you want to draw the menu window
    }

    void ButtonsAction(GLfloat* point) {
        // Implementation depends on how you want to handle button actions
    }

    void DialogsAction(GLfloat* point) {
        // Implementation depends on how you want to handle dialog actions
    }
};

// ����� ��� ��������� ����
class MainMenu {
private:
    std::vector<MenuWindow> windows; // Список всех окон меню

public:
    MainMenu() {}
    ~MainMenu() {}

    // Метод для добавления окна в список окон
    void addWindow(const MenuWindow& window) {
        windows.push_back(window);
    }

    // Метод для отрисовки всех окон
    void Draw() const {
        for (const auto& window : windows) {
            window.Draw();
        }
    }

    void OnMouseClick(GLfloat point[2]) {
        for (auto& window : windows) {
            window.ButtonsAction(point);
            window.DialogsAction(point);
        }
    }

    // Метод для чтения меню из файла
    void ReadMenuFromFile(const std::string& filename) {
        // Реализация зависит от формата файла
    }
};