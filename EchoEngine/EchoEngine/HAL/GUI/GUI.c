#include "GUI.h"
#include <stdlib.h>

struct GUI GUIInit(int x, int y) {
	struct GUI retValue = { x, y, NULL, NULL, 0, 2 };
	retValue.children = malloc(sizeof(struct Widget*) * 2);
	return retValue;
}
void GUIAddWidget(struct GUI* self, struct Widget* widget) {
	if (self->childrenCount == self->childrenCapacity) {
		self->children = realloc(self->children, sizeof(struct Widget*) * self->childrenCapacity * 2);
		self->childrenCapacity *= 2;
	}
	self->children[self->childrenCount] = widget;
	self->childrenCount++;
}



struct Widget GUICreateRect(struct Widget* parentWidget, struct GUI* parentGUI, int x, int y, int w, int h, void* userData) {
	struct Widget retValue = { x, y, w, h, W_RECT, userData, parentWidget, parentGUI, NULL, 0, 2 };
	retValue.children = malloc(sizeof(struct Widget*) * 2);
	return retValue;
}
struct Widget GUICreateTextBox(struct Widget* parentWidget, struct GUI* parentGUI, int x, int y, int w, void* userData) {
	struct Widget retValue = { x, y, w, 0, W_TEXTBOX, userData, parentWidget, parentGUI, NULL, 0, 2 };
	retValue.children = malloc(sizeof(struct Widget*) * 2);
	return retValue;
}
struct Widget GUICreateButton(struct Widget* parentWidget, struct GUI* parentGUI, int x, int y, int w, int h, void* userData) {
	struct Widget retValue = { x, y, w, h, W_BUTTON, userData, parentWidget, parentGUI, NULL, 0, 2 };
	retValue.children = malloc(sizeof(struct Widget*) * 2);
	return retValue;
}

void WidgetAddWidget(struct Widget* self, struct Widget* other) {
	if (self->childrenCount == self->childrenCapacity) {
		self->children = realloc(self->children, sizeof(struct Widget*) * self->childrenCapacity * 2);
		self->childrenCapacity *= 2;
	}
	self->children[self->childrenCount] = other;
	self->childrenCount++;
}

/*
void WidgetRender(struct Widget* self, int px, int py) {
	int newX = self->x + px;
	int newY = self->y + py;
	switch (self->wType) {
	case W_RECT:
		drawRect(newX, newY, self->w, self->h, 125, 125, 125, 255);
		break;
	case W_TEXTBOX: {
		struct TextBoxData {
			int fontWidth;
			const char* str;
		};
		struct TextBoxData* data = self->userData;
		drawText(data->str, newX, newY, data->fontWidth);
	}
		break;
	case W_BUTTON: {
		struct Data {
			void (*func)(struct Widget*, void*);
			void* param;
		};
		int cx, cy;
		getMouseCanvasPos(&cx, &cy);
		uint8_t leftButtonDown;
		getMouseState(&leftButtonDown, NULL, NULL);
		if (leftButtonDown) {
			if (cx < newX || cx > newX + self->w || cy < newY || cy > newY + self->h)
				break;
			struct Data* userData = self->userData;
			userData->func(self, userData->param);
		}
		break;
	}
	}
	for (unsigned int i = 0; i < self->childrenCount; i++) {
		WidgetRender(self->children[i], newX, newY);
	}
}
*/
void GUIRender(struct GUI* self) {
	for (unsigned int i = 0; i < self->childrenCount; i++) {
		WidgetRender(self->children[i], self->x, self->y);
	}
}