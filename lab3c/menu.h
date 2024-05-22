#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <GLUT/glut.h>

using namespace std;

class Color {
public:
    GLfloat r, g, b;
    Color(GLfloat _r, GLfloat _g, GLfloat _b) : r(_r), g(_g), b(_b) {}
    Color() : r(0.0f), g(0.0f), b(0.0f) {}
};

class Point {
public:
    GLfloat x, y;
    Point(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
    Point() : x(0.0f), y(0.0f) {}
};

class Size {
public:
    GLfloat width, height;
    Size(GLfloat _width, GLfloat _height) : width(_width), height(_height) {}
    Size() : width(0.0f), height(0.0f) {}
};

class MenuItem {
protected:
    Color color;
    Point position;
    Size size;

public:
    MenuItem(Color _color, Point _position, Size _size)
        : color(_color), position(_position), size(_size) {}
    MenuItem();
    virtual ~MenuItem() {}
    virtual void Draw(Point reference_point) const = 0;

    void setColor(Color _color) { color = _color; }
    Color getColor() const { return color; }
    void setPosition(Point _position) { position = _position; }
    Point getPosition() const { return position; }
    void setSize(Size _size) { size = _size; }
    Size getSize() const { return size; }
};

class Button : public MenuItem {
private:
    std::string name;
    Color highlightColor;
    bool pressed;
    bool active;

public:
    Button(Color _color, Point _position, Size _size, bool _active, std::string _name)
        : MenuItem(_color, _position, _size), name(_name), pressed(false), active(_active) {
        highlightColor = Color(1.0f, 1.0f, 1.0f);  
    }
    Button() : MenuItem(), name(""), pressed(false), active(false) { highlightColor = Color(1.0f, 0.0f, 0.0f); }
    ~Button() {}

    // Method for drawing the button overriden from MenuItem
    virtual void Draw(Point reference_point) const override;
    
    // Setters and getters for name, highlightColor and pressed
    // void setName(std::string _name) {
    //     name = _name;
    // }
    std::string getName() const {
        return name;
    }

    // void setHighlightColor(Color _highlightColor) {
    //     highlightColor = _highlightColor;
    // }
    // Color getHighlightColor() const {
    //     return highlightColor;
    // }

    void setPressed(bool _pressed) {
        pressed = _pressed;
    }
    // bool getPressed() const {
    //     return pressed;
    // }

    void setActive(bool _active) {
        active = _active;
    }

    bool getActive() const {
        return active;
    }

    // void ToggleState() {
    //     pressed = !pressed;
    // }
    // void SetPressed(bool isPressed) {
    //     pressed = isPressed;
    // }

    // bool IsPressed() const {
    //     return pressed;
    // }
};

class Dialog : public MenuItem {
private:
    std::string title;
    std::vector<Button> buttons;
    bool active;

public:
    Dialog(Color _color, Point _position, Size _size, std::string _title)
        : MenuItem(_color, _position, _size), title(_title), active(false) {}
    Dialog() : MenuItem(), title(""), active(false) {}
    ~Dialog() {}

    // Method for drawing the dialog overriden from MenuItem
    virtual void Draw(Point reference_point) const override;

    // Setters and getters for title, buttons and active
    // void setTitle(std::string _title) { title = _title; }
    std::string getTitle() const { return title; }

    // void setButtons(std::vector<Button> _buttons) { buttons = _buttons; }
    std::vector<Button> getButtons() const { return buttons; }

    void setActive(bool _active) { active = _active; }
    // bool getActive() const { return active; }

    // void ToggleState() { active = !active; }
    // void SetActive(bool isActive) { active = isActive; }
    // bool IsActive() const { return active; }

    void AddButton(Button button) { buttons.push_back(button); }
};

class MenuWindow : public MenuItem {
private:
    std::vector<Button> buttons;
    std::vector<Dialog> dialogs;

public:
    MenuWindow(Color _color, Point _position, Size _size)
        : MenuItem(_color, _position, _size) {}
    MenuWindow() : MenuItem() {}

    // void setColor(Color _color) { color = _color; }
    // Color getColor() const { return color; }

    // void setPosition(Point _position) { position = _position; }
    // Point getPosition() const { return position; }

    // void setSize(Size _size) { size = _size; }
    // Size getSize() const { return size; }

    void AddButton(const Button& button) { buttons.push_back(button); }

    void AddDialog(const Dialog& dialog) { dialogs.push_back(dialog); }
    
    // vector<Button>& getButtons() { return buttons; }
    // vector<Dialog>& getDialogs() { return dialogs; }

    virtual void Draw(Point reference_point) const override;

    void ButtonsAction(Point point);

    void DialogsAction(Point point);
};

class MainMenu {
private:
    MenuWindow menu_window;
    
public:
    MainMenu(){};
    void addWindow(MenuWindow window);

    void Draw(Point mouse_position) const;

    void OnMouseClick(int button, int state, int x, int y);

    void ReadMenuFromFile(const std::string& filename);
};