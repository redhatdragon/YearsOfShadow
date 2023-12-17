#pragma once

#include "HAL/HAL.h"
#include "EchoEngine/Widget.h"

struct TextButtonData {
	HAL::texture_handle_t textureOnDraw;
	HAL::texture_handle_t textureOnDown;
	HAL::texture_handle_t textureOnHover;
	char* text;
	uint32_t fontSize;
};
std::vector<std::string> splitString(const char* input, char delim) {
	uint32_t i = 0;
	std::vector<std::string> retValue = {};
	std::string currentString = "";
	while (true) {
		if (input[i] == 0) {
			retValue.push_back(currentString);
			return retValue;
		}
		if (input[i] == '\n') {
			retValue.push_back(currentString);
			currentString = "";
			i++;
			continue;
		}
		currentString += input[i];
		i++;
	}
}
void drawTextButton(Widget* self) {
	int x = self->x, y = self->y, w = self->w, h = self->h;
	TextButtonData* data = (TextButtonData*)self->data;
	uint32_t strLen = (uint32_t)strlen(data->text);
	uint32_t endlineCount = 0;
	for (uint32_t i = 0; i < strLen; i++)
		if (data->text[i] == '\n')
			endlineCount++;
	//h += data->fontSize * endlineCount;
	//TODO: this will have a bug where clicking before hoving would trigger this still...
	auto strings = splitString(data->text, '\n');
	if (self->state == Widget::PRESSED) {
		HAL::draw_texture(data->textureOnDown, x, y, w, h);
		HAL::draw_text(strings[0].c_str(), x+2, y+2, data->fontSize);
		for (uint32_t i = 0; i < endlineCount; i++) {
			HAL::draw_text(strings[i+1].c_str(), x+2, y+2+1 + data->fontSize*(i+1), data->fontSize);
		}
		//throw;
		return;
	}
	auto pos = HAL::get_mouse_canvas_pos();
	if (Widget::pointInWidget(self, pos.x, pos.y)) {
		HAL::draw_texture(data->textureOnHover, x, y, w, h);
		HAL::draw_text(strings[0].c_str(), x+2, y+2, data->fontSize);
		for (uint32_t i = 0; i < endlineCount; i++) {
			HAL::draw_text(strings[i + 1].c_str(), x + 2, y + 2 + 1 + data->fontSize * (i + 1), data->fontSize);
		}
		return;
	}
	HAL::draw_texture(data->textureOnDraw, x, y, w, h);
	HAL::draw_text(strings[0].c_str(), x+2, y+2, data->fontSize);
	for (uint32_t i = 0; i < endlineCount; i++) {
		HAL::draw_text(strings[i + 1].c_str(), x + 2, y + 2 + 1 + data->fontSize * (i + 1), data->fontSize);
	}
}
void cleanupTextButton(Widget* self) {
	TextButtonData* data = (TextButtonData*)self->data;
	free(data->text);
	free(data);
}
Widget* makeTextButton(const char* str, int x, int y,
	const char* _textureOnDraw, const char* _textureOnDown, const char* _textureOnHover, uint32_t fontSize)
{
	HAL::texture_handle_t textureOnDraw = HAL::get_new_texture(_textureOnDraw);
	HAL::texture_handle_t textureOnDown = HAL::get_new_texture(_textureOnDown);
	HAL::texture_handle_t textureOnHover = HAL::get_new_texture(_textureOnHover);
	int w = fontSize * (int)strlen(str) + 4, h = fontSize + 4;
	Widget* textButton = Widget::createNew(x, y, w, h);
	textButton->onDisplay = drawTextButton;
	textButton->cleanUpData = cleanupTextButton;
	TextButtonData* tbData;
	HAL_ALLOC_TYPE(tbData);
	char* text;
	uint32_t len = (uint32_t)strlen(str);
	HAL_ALLOC_RAWBYTE(text, len+1);
	memcpy(text, str, len+1);
	uint32_t endlineCount = 0;
	for (uint32_t i = 0; i < len; i++)
		if (text[i] == '\n')
			endlineCount++;
	textButton->h += fontSize * endlineCount;
	*tbData = { textureOnDraw, textureOnDown, textureOnHover, text, fontSize };
	textButton->data = tbData;
	return textButton;
}



//Widget* wMainMenu;
void initMainMenu() {
	//wMainMenu = Widget::createNew(0, 0, 0, 0);
	const char* buttonDrawPath = "./Data/Textures/grass1.png";
	const char* buttonDownPath = "./Data/Textures/Grass1_n.png";
	const char* buttonHoverPath = "./Data/Textures/Bullet.png";
	Widget* playButton = makeTextButton("Play\nIt\nNow", 200, 200, buttonDrawPath, buttonDownPath, buttonHoverPath, 64);
	//Widget::parentTo(wMainMenu, playButton);
}

void tickMainMenu() {
	auto mPos = HAL::get_mouse_canvas_pos();
	bool leftClicked = HAL::get_mouse_left_state();
	Widget::update(mPos.x, mPos.y, leftClicked);
	Widget::display();
}