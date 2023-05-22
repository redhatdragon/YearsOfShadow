#pragma once

/*#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_ttf.lib")*/

/*#pragma comment(lib, "lib/x86/SDL2.lib")
#pragma comment(lib, "lib/x86/SDL2main.lib")
#pragma comment(lib, "lib/x86/SDL2_image.lib")
#pragma comment(lib, "lib/x86/SDL2_ttf.lib")*/


#include "../../IO_API.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ThreadPool.h"

#ifdef USING_GL
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#endif

#include <time.h>
#include <stdio.h>
#include <Windows.h>
//#include <memory.h>

#ifdef DIR_TO_DATA 
#error "DIR_TO_DATA already defined"
#else
#define DIR_TO_DATA "../data/"
#endif

struct SDL_Window *window;
struct SDL_Renderer *renderer;
const Uint8* keys;

float FPS;
float FPSLimit = 30;

void drawTexture(void* texture, int x, int y, int w, int h) {
	struct SDL_Texture* t = (struct SDL_Texture*)texture;
	struct SDL_Rect r = { x, y, w, h };
	SDL_RenderCopy(renderer, t, NULL, &r);
}
void recolorTexture(void* texture, uint8_t r, uint8_t g, uint8_t b) {
	SDL_SetTextureColorMod((SDL_Texture*)texture, r, g, b);
}
void* getTexture(const char* fileName) {
	//struct *t = IMG_LoadTexture(renderer, fileName);  //This should be more optimized but fails in debug mode with magic number issue.
	struct SDL_Surface* s = IMG_Load(fileName);
	void* retValue = SDL_CreateTextureFromSurface(renderer, s);
	SDL_FreeSurface(s); 
	return retValue;
}
void releaseTexture(void* texture) {
	SDL_DestroyTexture((struct SDL_Texture*)texture);
}
void drawText(const char* str, int x, int y, unsigned int fontWidth) {
	static unsigned int lastFontWidth = 0;
	static TTF_Font* font = NULL;
	if (fontWidth == 0) return;
	if (fontWidth != lastFontWidth) {
		if (font) TTF_CloseFont(font);
		font = TTF_OpenFont(DIR_TO_DATA "fonts/consola.ttf", fontWidth);
		if (font == NULL)
			printf("Error: Can't load font arial.ttf.\n");
		lastFontWidth = fontWidth;
	}

	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, str, color);
	surface->refcount++;  //Added to prevent crash in debug
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int strLen = (int)strlen(str);
	struct SDL_Rect r = { x, y, fontWidth*strLen, fontWidth };
	SDL_RenderCopy(renderer, texture, NULL, &r);


	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}
void drawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	//SDL_Color color = { 255, 255, 255 };
	//SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, y);
	struct SDL_Rect rect = { x, y, w, h };
	//SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


	//SDL_DestroyTexture(texture);
}
void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawPoint(renderer, x, y);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
void drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

bool getKeyState(char k) {
	return keys[k+(SDL_SCANCODE_A - 'a')];
}
void getMouseState(uint8_t* leftButton, uint8_t* middleButton, uint8_t* rightButton) {
	Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
	if (leftButton) {
		*leftButton = false;
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
			*leftButton = true;
	}
	if (middleButton) {
		*middleButton = false;
		if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
			*middleButton = true;
	}
	if (rightButton) {
		*rightButton = false;
		if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
			*rightButton = true;
	}
}
void getMouseCanvasPos(int* x, int* y) {
	SDL_GetMouseState(x, y);
}

void getCanvasSize(unsigned int* width, unsigned int* height) {
	*width = 1024; *height = 768;
}

bool getFileText(const char* fileName, char* strBuffer) {
	FILE* t = fopen(fileName, "r");
	if (t == NULL) return false;
	char c;
	size_t i = 0;
	while ((c = getc(t)) != EOF) {
		strBuffer[i] = c;
		i++;
	}
	fclose(t);
	return true;
}
uint32_t fileGetSize(const char* fileName) {
	FILE* t = fopen(fileName, "r");
	if (t == NULL) return 0;
	fseek(t, 0L, SEEK_END);
	uint32_t retValue = ftell(t);
	fseek(t, 0L, SEEK_SET);
	fclose(t);
	return retValue;
}
const char* getDirData() {
	return DIR_TO_DATA;
}

float getFPS() {
	return FPS;
}
void setFPS(uint32_t fps) {
	FPSLimit = fps;
}

void initAsNormal() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	window = SDL_CreateWindow("Some window title", 60, 40, 1024, 768, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	if (IMG_Init(IMG_INIT_PNG) == 0)
		printf("Failed to initialize SDL_Image");
	if (TTF_Init() != 0)
		printf("Failed to initialize SDL_ttf");
	printf("CPU cache line size: %d", SDL_GetCPUCacheLineSize());
	printf("\n");
	printf("CPU core count: %d", SDL_GetCPUCount());
	printf("\n");
	printf("System RAM: %d", SDL_GetSystemRAM());
	printf("\n");
}
void initAsGL() {
	const SDL_VideoInfo* info = NULL;
	int width = 0;
	int height = 0;
	/* Color depth in bits of our window. */
	int bpp = 0;
	/* Flags we will pass into SDL_SetVideoMode. */
	int flags = 0;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n",
			SDL_GetError());
	}
	info = SDL_GetVideoInfo();

	if (!info) {
		fprintf(stderr, "Video query failed: %s\n",
			SDL_GetError());
	}
	width = 640;
	height = 480;
	bpp = info->vfmt->BitsPerPixel;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/*
	 * EXERCISE:
	 * Make starting windowed an option, and
	 * handle the resize events properly with
	 * glViewport.
	 */
	flags = SDL_OPENGL | SDL_FULLSCREEN;

	/*
	 * Set the video mode
	 */
	if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n",
			SDL_GetError());
	}



	if (IMG_Init(IMG_INIT_PNG) == 0)
		printf("Failed to initialize SDL_Image");
	if (TTF_Init() != 0)
		printf("Failed to initialize SDL_ttf");
	printf("CPU cache line size: %d", SDL_GetCPUCacheLineSize());
	printf("\n");
	printf("CPU core count: %d", SDL_GetCPUCount());
	printf("\n");
	printf("System RAM: %d", SDL_GetSystemRAM());
	printf("\n");
}

int main(int argc, char *argv[]) {
#if USING_GL
	initAsGL();
#else
	initAsNormal();
#endif
	appStart();



	gameLoop:
	{
		clock_t startTime = clock();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				goto quit;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
				goto quit;
		}
		keys = SDL_GetKeyboardState(NULL);
		SDL_RenderClear(renderer);
		appLoop();

		SDL_RenderPresent(renderer);
		//const uint32_t mili_per_frame = 16*2;
		uint32_t mili_per_frame = (1.f / FPSLimit)*1000;
		clock_t endTime = clock();
		clock_t differentialTime = endTime - startTime;
		if (differentialTime < mili_per_frame) {
			SDL_Delay(mili_per_frame - differentialTime);
			differentialTime = clock() - startTime;
		}
		FPS = (double)1000/((double)differentialTime);
	} goto gameLoop;

quit:
	appEnd();
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}