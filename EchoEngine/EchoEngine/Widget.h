#pragma once
#include <vector>

struct Widget {
    static int mouseX, mouseY;
    static bool leftMousePressed;
    static bool leftMouseClicked;
    static Widget* pressedWidget;
    static Widget* releasedWidget;
    static std::vector<Widget*> widgets;

    size_t id;
    size_t type;
    size_t layer = 0;
    enum {
        IDLE,
        CLICKED,
        PRESSED,
        RELEASED
    };
    uint8_t state = IDLE;
    int x, y, w, h;
    void(*onClicked)(Widget* self) = 0;
    void(*onPressed)(Widget* self) = 0;
    void(*onReleased)(Widget* self) = 0;
    void(*onDisplay)(Widget* self) = 0;
    void(*cleanUpData)(Widget* self) = 0;
    void* data;
    Widget* parent;
    std::vector<Widget*> children;

    static Widget* createNew(int _x, int _y, int _w, int _h) {
        Widget* retValue = (Widget*)malloc(sizeof(Widget));
        if (retValue == NULL) throw;
        memset(retValue, 0, sizeof(Widget));
        retValue->x = _x, retValue->y = _y, retValue->w = _w, retValue->h = _h;
        retValue->id = widgets.size();
        widgets.push_back(retValue);
        return retValue;
    }
    static void destruct(Widget* widget) {
        if (widget->cleanUpData)
            widget->cleanUpData(widget);
        widgets[widget->id] = widgets[widgets.size() - 1];
        widgets[widget->id]->id = widget->id;
        free(widget);
        widgets.pop_back();
    }

    static void parentTo(Widget* parent, Widget* child) {
        child->parent = parent;
        child->layer = parent->layer + 1;
        parent->children.push_back(child);
    }

    static void update(int _mouseX, int _mouseY, bool _leftMousePressed) {
        updateInputState(_mouseX, _mouseY, _leftMousePressed);
        if (releasedWidget) {
            releasedWidget->state = IDLE;
            releasedWidget = nullptr;
        }
        if (pressedWidget) {
            if (leftMousePressed == false || Widget::pointInWidget(pressedWidget, _mouseX, _mouseY) == false) {
                releasedWidget = pressedWidget;
                pressedWidget->onReleased(pressedWidget);
                releasedWidget->state = RELEASED;
                pressedWidget = nullptr;
            }
        }
        bool pressedWidgetWasJustClicked = false;
        for (Widget* w : widgets) {
            if (pointInWidget(w, mouseX, mouseY)) {
                if (leftMouseClicked) {
                    if (pressedWidget == nullptr) {
                        pressedWidget = w;
                        w->state = CLICKED;
                        pressedWidgetWasJustClicked = true;
                    }
                    if (w->onClicked != 0)
                        w->onClicked(w);
                }
            }
        }
        if (pressedWidget) {
            if (pressedWidgetWasJustClicked == false)
                pressedWidget->state = PRESSED;
            if (pressedWidget->onPressed)
                pressedWidget->onPressed(pressedWidget);
        }
    }
    static void display() {
        for (Widget* w : widgets) {
            if (w->layer == 0) {
                displayRecursivly(w);
            }
        }
    }
    static bool pointInWidget(Widget* self, int px, int py) {
        if (px > self->x + self->w || px < self->x ||
            py > self->y + self->h || py < self->y)
            return false;
        return true;
    }
private:
    static void updateInputState(int _mouseX, int _mouseY, bool _leftMousePressed) {
        mouseX = _mouseX;
        mouseY = _mouseY;
        if (leftMousePressed == false && _leftMousePressed == true) {
            leftMouseClicked = true;
            leftMousePressed = _leftMousePressed;
            return;
        }
        if (leftMousePressed == true) {
            leftMouseClicked = false;
        }
        leftMousePressed = _leftMousePressed;
    }
    static void displayRecursivly(Widget *widget) {
        if (widget->onDisplay)
            widget->onDisplay(widget);
        for (Widget* child : widget->children)
            displayRecursivly(child);
    }
};