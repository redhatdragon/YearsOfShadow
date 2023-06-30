#include "Widget.h"

int Widget::mouseX = 0;
int Widget::mouseY = 0;
bool Widget::leftMousePressed = false;
bool Widget::leftMouseClicked = false;
Widget* Widget::pressedWidget = nullptr;
Widget* Widget::releasedWidget;
std::vector<Widget*> Widget::widgets = std::vector<Widget*>();