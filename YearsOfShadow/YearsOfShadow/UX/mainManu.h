#pragma once

#include "HAL/HAL.h"
#include "CustomWidgets.h"



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