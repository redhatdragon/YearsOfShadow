#pragma once
#include "EchoEngine/Widget.h"

struct TextButtonData {
	HAL::texture_handle_t textureOnDraw;
	HAL::texture_handle_t textureOnDown;
	HAL::texture_handle_t textureOnHover;
	char* text;
	uint32_t fontSize;
};
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
	auto strings = HAL::splitString(data->text, '\n');
	if (self->state == Widget::PRESSED) {
		HAL::draw_texture(data->textureOnDown, x, y, w, h);
		HAL::draw_text_multi_line(data->text, x+2, y+2, data->fontSize, 0);
		//throw;
		return;
	}
	auto pos = HAL::get_mouse_canvas_pos();
	if (Widget::pointInWidget(self, pos.x, pos.y)) {
		HAL::draw_texture(data->textureOnHover, x, y, w, h);
		HAL::draw_text_multi_line(data->text, x + 2, y + 2, data->fontSize, 0);
		return;
	}
	HAL::draw_texture(data->textureOnDraw, x, y, w, h);
	HAL::draw_text_multi_line(data->text, x + 2, y + 2, data->fontSize, 0);
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



struct InputTextButtonData {
	HAL::texture_handle_t textureOnDraw;
	HAL::texture_handle_t textureOnDown;
	HAL::texture_handle_t textureOnHover;
	char* text;
	uint32_t fontSize;
};
void drawInputTextButton(Widget* self) {
	int x = self->x, y = self->y, w = self->w, h = self->h;
	TextButtonData* data = (TextButtonData*)self->data;
	uint32_t strLen = (uint32_t)strlen(data->text);
	uint32_t endlineCount = 0;
	for (uint32_t i = 0; i < strLen; i++)
		if (data->text[i] == '\n')
			endlineCount++;
	//h += data->fontSize * endlineCount;
	//TODO: this will have a bug where clicking before hoving would trigger this still...
	auto strings = HAL::splitString(data->text, '\n');
	if (self->state == Widget::PRESSED) {
		HAL::draw_texture(data->textureOnDown, x, y, w, h);
		HAL::draw_text_multi_line(data->text, x + 2, y + 2, data->fontSize, 0);
		//throw;
		return;
	}
	auto pos = HAL::get_mouse_canvas_pos();
	if (Widget::pointInWidget(self, pos.x, pos.y)) {
		HAL::draw_texture(data->textureOnHover, x, y, w, h);
		HAL::draw_text_multi_line(data->text, x + 2, y + 2, data->fontSize, 0);
		return;
	}
	HAL::draw_texture(data->textureOnDraw, x, y, w, h);
	HAL::draw_text_multi_line(data->text, x + 2, y + 2, data->fontSize, 0);
}
void cleanupInputTextButton(Widget* self) {
	TextButtonData* data = (TextButtonData*)self->data;
	free(data->text);
	free(data);
}
Widget* makeInputTextButton(const char* str, int x, int y,
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
	HAL_ALLOC_RAWBYTE(text, len + 1);
	memcpy(text, str, len + 1);
	uint32_t endlineCount = 0;
	for (uint32_t i = 0; i < len; i++)
		if (text[i] == '\n')
			endlineCount++;
	textButton->h += fontSize * endlineCount;
	*tbData = { textureOnDraw, textureOnDown, textureOnHover, text, fontSize };
	textButton->data = tbData;
	return textButton;
}