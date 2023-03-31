#pragma once

/*
NOTES:
So something important to understand, this is meant to be C compatible.

Screen space is in cartesian coordinates.  Relative to window dimensions.
Example: if winWidth and winHeight == 400;  200, 200 is the cnter of the window.
In the 3D extension of this API, +Z is away from the camera, -Z is towards it.

For now keyboard values are 1:1 with the ascii table.  Ref: https://www.asciitable.com/

TODO: Decide on a better naming convention for this API.
Yaaaaaaay breaking API changes coming soon!
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef __cplusplus
#ifndef _STDBOOL
#define _STDBOOL
	typedef uint8_t bool;
#define true 1
#define false 0
#endif
#endif



	//void drawTextureFromFile(const char* fileName, int x, int y);
	void* EE_getNewTexture(const char* fileName);
	//void resizeTexture(void* texture, uint32_t width, uint32_t height);
	void EE_drawTexture(void* texture, int x, int y, int w, int h);
	void EE_recolorTexture(void* texture, uint8_t r, uint8_t g, uint8_t b);
	void EE_releaseTexture(void* texture);
	void EE_drawBackground(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void EE_drawText(const char* str, int x, int y, unsigned int fontWidth);
	void EE_drawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void EE_drawBloom(int x, int y, int w, int h, int intensity);
	void EE_drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void EE_drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	//const uint8_t* getKeyboardState();
	bool EE_getKeyState(char k);
	void EE_getMouseState(uint8_t* leftButton, uint8_t* middleButton, uint8_t* rightButton);
	void EE_getMouseCanvasPos(int* x, int* y);
	void EE_setMouseCanvasPos(int x, int y);
	void EE_setMouseCursorHide(uint8_t isTrue);
	void EE_setMouseEnable(uint8_t isTrue);

	//void getWindowSize(unsigned int *width, unsigned int *height);
	//void setWindowSize(unsigned int width, unsigned int height);
	void EE_getCanvasSize(unsigned int* width, unsigned int* height);
	void EE_setCanvasSize(unsigned int width, unsigned int height);

	bool EE_getFileData(const char* fileName, uint8_t* dataBuffer);
	bool EE_getFileDataPartial(const char* fileName, uint8_t* dataBuffer, uint32_t startPos, uint32_t numBytes);
	bool EE_getFileText(const char* fileName, char* strBuffer);
	bool EE_writeFileData(const char* fileName, uint8_t* data);
	bool EE_writeFileText(const char* fileName, char* str);
	uint32_t EE_fileGetSize(const char* fileName);
	const char* EE_getDirData();

	float EE_getFPS();
	void EE_limitFPS(uint32_t fps);

	bool EE_playAudioFile(const char* fileName, uint8_t loop);
	bool EE_getNewAudio(const char* fileName, void* audioBuffer);
	bool EE_playAudio(void* audio, uint8_t loop);
	bool EE_stopAudio(void* audio);

	bool EE_sendPacketUDP(void* packet);
	struct EE_PacketUDP* EE_recvPacketUDP();

	void* EE_getNewThreadPool(uint16_t maxThreadCount);
	void EE_releaseThreadPool(void* self);
	uint16_t EE_getThreadPoolFreeThreadCount(void* self);
	bool EE_sendThreadPoolTask(void* self, void(*func)(void*), void* param);
	bool EE_isThreadPoolFinished(void* self);



	//Define these in your application to be called by the HAL_ implimentations.
	void EE_appStart();
	void EE_appLoop();
	void EE_appEnd();



	#define EE_KEY_ESC 27
	#define EE_KEY_SPACE 32
	#define EE_KEY_TAB 9

#ifdef __cplusplus
}
#endif