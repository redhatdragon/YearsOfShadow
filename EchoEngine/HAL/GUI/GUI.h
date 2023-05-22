#pragma once
#include "../HAL.h"

#ifdef __cplusplus
extern "C" {
#endif

enum WIDGET_TYPE {
	W_RECT,
	W_TEXTBOX,
	W_BUTTON
};

struct Widget;

struct GUI {
	int x, y;
	struct Widget* active;
	struct Widget** children;
	unsigned int childrenCount, childrenCapacity;
};

struct Widget {
	int x, y, w, h;
	enum WIDGET_TYPE wType;
	void* userData;
	struct Widget* parentWidget;
	struct GUI* parentGUI;
	struct Widget** children;
	unsigned int childrenCount, childrenCapacity;
};

struct GUI GUIInit(int x, int y);
void GUIAddWidget(struct GUI* self, struct Widget* widget);

struct Widget GUICreateRect(struct Widget* parentWidget, struct GUI* parentGUI,int x, int y, int w, int h, void* userData);
struct Widget GUICreateTextBox(struct Widget* parentWidget, struct GUI* parentGUI, int x, int y, int w, void* userData);
struct Widget GUICreateButton(struct Widget* parentWidget, struct GUI* parentGUI, int x, int y, int w, int h, void* userData);



void WidgetAddWidget(struct Widget* self, struct Widget* other);

void WidgetRender(struct Widget* self, int px, int py);
void GUIRender(struct GUI* self);

#ifdef __cplusplus
}
#endif