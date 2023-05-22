#pragma once

/*************************************************************************
				[PUBLIC DOMAIN]

				WIN32 IO API

Build:	Build in VS or your favorite compiler as a win32 application.
You may compile as a console application by using
#define IS_CONSOLE_APPLICATION
For VS you must disable CRT WARNINGS by going into the 
project properties->C/C++->General->SDL checks and "set to no".

Designed with windows needs in mind, this file conforms to the IO_API api.
No additional code tweeks required.

This lib should come with a folder called "Texture" for png loading.

TODO:
Create functions for file, audio, and networking IO.

TESTED WITH:
Windows 7 64 Ultimate
	-With Visual Studio 2017 x86 release mode
*************************************************************************/

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#define CUTE_SOUND_IMPLEMENTATION

#include "IO_API.h"
#include "Texture/picoPNG/picoPNG.h"
#include "cute_sound.h"
//#include "mp3ToWav.h"
//#define MINIMP3_ONLY_MP3
//#define MINIMP3_ONLY_SIMD
//#define MINIMP3_NO_SIMD
//#define MINIMP3_NONSTANDARD_BUT_LOGICAL
//#define MINIMP3_FLOAT_OUTPUT
//#define MINIMP3_IMPLEMENTATION
//#include "minimp3.h"
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <time.h>
#include <stdio.h>

struct ColorF {
	float r, g, b, a;
};
struct ColorB {
	uint8_t r, g, b, a;
};

struct ColorF canvas[1024 * 768];
uint16_t cWidth = 0;
uint16_t cHeight = 0;

HWND windowHandle;



/*void drawTextureFromFile(const char *fileName, int x, int y) {
	typedef unsigned int uint;
	uint32_t *buffer = 0;
	unsigned int width, height;
	getPNGData(fileName, &buffer, &width, &height);
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			if (!((uint8_t*)&buffer[i + j * width])[4])
				continue;

			unsigned int px = i + x, py = j + y;

			if (px < (uint)0 || py < (uint)0 || px > cWidth - (uint)1 || py > cHeight - (uint)1)
				continue;

			canvas[px + py * cWidth] = buffer[i + j * width];
		}
	}
	free(buffer);
}*/
/*void drawTextureFromFileUnsafe(const char *fileName, int x, int y) {
	uint32_t *buffer = NULL;
	unsigned int width, height;
	getPNGData(fileName, &buffer, &width, &height);
	for (unsigned int i = 0; i < height; i++) {
		uint32_t *loc = &canvas[x + (i + y) * cWidth];
		memcpy(loc, &buffer[i*width], width * sizeof(uint32_t));
	}
	free(buffer);
}*/
void drawTexture(const struct Texture *texture, int _x, int _y) {
	uint32_t *buffer = texture->data;
	unsigned int width = texture->w, height = texture->h;
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			int x = i + _x, y = j + _y;
			//if (!((uint8_t*)&buffer[i + j * width])[4])
			struct P {
				uint8_t r, g, b, a;
			};
			struct P* pixels = (struct P*)buffer;
			if (pixels[i+j*width].a == 0)
				continue;
			if (x < 0 || y < 0 || x > cWidth - 1 || y > cHeight - 1)
				continue;
			canvas[x + y * cWidth] = buffer[i + j * width];
		}
	}
}
void getTexture(const char *fileName, struct Texture *texture) {
	uint32_t *buffer = NULL;
	getPNGData(fileName, &buffer, &texture->w, &texture->h);
	for (uint32_t i = 0; i < texture->w * texture->h; i++) {
		struct ColorB p;
		memcpy(&p, &buffer[i], sizeof(struct ColorB));
		uint8_t r = p.r;
		p.r = p.b;
		p.b = r;
		struct ColorF pf;
		pf.r = p.r; pf.g = p.g;  pf.b = p.b;
		memcpy(&buffer[i], &pf, sizeof(struct ColorF));
	}
	texture->data = buffer;
}
void resizeTexture(struct Texture* input, uint32_t width, uint32_t height) {
	struct Texture output;
	output.w = width; output.h = height;
	output.data = (uint32_t*)malloc(width * height * sizeof(uint32_t));

	int32_t w1, h1, w2, h2;
	w1 = input->w; h1 = input->h;
	w2 = width; h2 = height;
	int32_t x_ratio = (int32_t)((w1 << 16) / w2) + 1;
	int32_t y_ratio = (int32_t)((h1 << 16) / h2) + 1;
	int32_t x2, y2;
	for (int32_t i = 0; i < h2; i++) {
		for (int32_t j = 0; j < w2; j++) {
			x2 = ((j * x_ratio) >> 16);
			y2 = ((i * y_ratio) >> 16);
			output.data[(i * w2) + j] = input->data[(y2 * w1) + x2];
		}
	}
	input->w = width; input->h = height;
	free(input->data);
	input->data = output.data;
}
void drawText(const char* str, int x, int y, unsigned int fontWidth) {
	struct Texture font;
	getTexture("Font/MenuFont/MenuFont.png", &font);
	struct Texture glyph[128];
	for (unsigned int i = 0; i < 128; i++) {
		glyph[i].w = 8;
		glyph[i].h = 16;
		glyph[i].data = malloc(8 * 16 * sizeof(uint32_t));
		memset(glyph[i].data, 0, 8 * 16 * sizeof(uint32_t));
		for (unsigned int j = 0; j < 16; j++) {
			memcpy(&glyph[i].data[j*8], &font.data[(i*8)+font.w*j], 8*sizeof(uint32_t));
		}
		resizeTexture(&glyph[i], fontWidth, fontWidth * 2);
	}
	{
		size_t i = 0;
		while (true) {
			if (str[i] == 0) break;
			drawTexture(&glyph[str[i]-' '], x+i*(fontWidth), y);
			i++;
		}
	}
	for (unsigned int i = 0; i < 128; i++) {
		free(glyph[i].data);
	}
	free(font.data);
}
void drawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	uint8_t color[4] = { b, g, r, a };
	for (unsigned int i = 0; i < w; i++) {
		for (unsigned int j = 0; j < h; j++) {
			int newX = i+x; int newY = j+y;
			if(newX < 0 || newX > cWidth
			|| newY < 0 || newY > cHeight)
				continue;
			memcpy(&canvas[newX + newY * cWidth], &color[0], sizeof(uint32_t));
		}
	}
}
void drawBloom(int x, int y, int w, int h, int intensity) {
	struct Texture kernel;
	kernel.w = w; kernel.h = h;
	kernel.data = malloc(w*h*sizeof(uint32_t));
	for (int i = 0; i < h; i++) {
		memcpy(&kernel.data[0+i*w], &canvas[x + (y+i) * cWidth], w*sizeof(uint32_t));
	}
	resizeTexture(&kernel, w / intensity, h / intensity);
	resizeTexture(&kernel, w, h);
	for (int i = y; i < y+h; i++) {
		for (int j = x; j < x+w; j++) {
			canvas[j+i*cWidth] += kernel.data[(j-x)+(i-y)*w];
		}
		//memcpy(&canvas[x + i * cWidth], &kernel.data[0 + i * w], w*sizeof(uint32_t));
	}
	free(kernel.data);
}

bool keyStates[256];
void getKeyboardState(uint8_t *outBuffer) {
	for (unsigned int i = 0; i < 256; i++) {
		//outBuffer[i] = GetAsyncKeyState(i) & 0x8000;
		//outBuffer[i] = GetAsyncKeyState(i) & 0x0001;
	}
	//GetKeyboardState(outBuffer);
	memcpy(outBuffer, keyStates, 256);
}
void getMouseState(uint8_t* leftButton, uint8_t* middleButton, uint8_t* rightButton) {
	uint8_t left, middle, right;
	left = middle = right = false;
	if (GetKeyState(VK_LBUTTON) < 0)
		left = true;
	if (GetKeyState(VK_MBUTTON) < 0)
		middle = true;
	if (GetKeyState(VK_RBUTTON) < 0)
		right = true;
	if (leftButton != NULL)
	*leftButton = left;
	if (middleButton != NULL)
	*middleButton = middle;
	if (rightButton != NULL)
	*rightButton = right;
}
void getMouseCanvasPos(int* retX, int* retY) {
	POINT p;
	if (GetCursorPos(&p)) {
		if (ScreenToClient(windowHandle, &p)) {
			*retX = p.x;
			*retY = p.y;
			return;
		}
	}
	*retX = -1;
	*retY = -1;
}

bool getFileData(const char* fileName, uint8_t* dataBuffer, uint32_t startPos, uint32_t numBytes) {
	FILE *file = fopen(fileName, "r+");
	if (!file) {
		printf("getFileData: Failed to open file.");
		return false;
	}
	fseek(file, startPos, SEEK_SET);
	size_t bytesRead = fread(dataBuffer, 1, numBytes, file);
	fclose(file);
	if (bytesRead)
		return true;
	printf("getFileData: failed to read file.");
	return false;
}

cs_context_t* ctx;
cs_playing_sound_t *activeSound[256];
uint32_t activeSoundCount = 0;
bool playAudioFile(const char* fileName, uint8_t loop) {
	//PlaySound(TEXT(fileName), NULL, SND_FILENAME | SND_ASYNC);

	/*FILE* file = fopen(fileName, "r");
	fseek(file, 0, SEEK_END);
	unsigned int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	uint8_t* input_buf = malloc(fileSize);
	fclose(file);

	static mp3dec_t mp3d;
	mp3dec_init(&mp3d);
	mp3dec_frame_info_t info;
	short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME] = { 0 };
	int readSamples = mp3dec_decode_frame(&mp3d, input_buf, fileSize, pcm, &info);

	cs_loaded_sound_t tAudio;
	tAudio.sample_rate = info.hz;
	tAudio.sample_count = readSamples;
	tAudio.playing_count = 0;
	tAudio.channel_count = info.channels;
	tAudio.channels[0] = malloc(MINIMP3_MAX_SAMPLES_PER_FRAME * 2);
	memcpy(tAudio.channels[0], pcm, MINIMP3_MAX_SAMPLES_PER_FRAME * 2);
	cs_loaded_sound_t* audio = malloc(sizeof(struct cs_loaded_sound_t));
	memcpy(audio, &tAudio, sizeof(struct cs_loaded_sound_t));
	cs_playing_sound_t audioInstance = cs_make_playing_sound(audio);
	activeSound[activeSoundCount] = audioInstance;
	cs_insert_sound(ctx, &activeSound[activeSoundCount]);
	activeSoundCount++;*/

	cs_loaded_sound_t tAudio = cs_load_wav(fileName);
	cs_loaded_sound_t* audio = malloc(sizeof(struct cs_loaded_sound_t));
	memcpy(audio, &tAudio, sizeof(struct cs_loaded_sound_t));
	cs_playing_sound_t tAudioInstance = cs_make_playing_sound(audio);
	if (loop)
		tAudioInstance.looped = true;
	cs_playing_sound_t* audioInstance = malloc(sizeof(cs_playing_sound_t));
	memcpy(audioInstance, &tAudioInstance, sizeof(tAudioInstance));
	activeSound[activeSoundCount] = audioInstance;
	cs_insert_sound(ctx, activeSound[activeSoundCount]);
	activeSoundCount++;
}



void PaintWindow(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);
	HDC hDCMem = CreateCompatibleDC(hDC);

	unsigned char* lpBitmapBits;

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = cWidth;
	bi.bmiHeader.biHeight = -cHeight;  //negative so (0,0) is at top left
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;


	HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
	HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);

	BitBlt(hDCMem, 0, 0, cWidth, cHeight, hDC, 0, 0, SRCCOPY);

	memcpy(lpBitmapBits, canvas, cWidth*cHeight*4);

	BitBlt(hDC, 0, 0, cWidth, cHeight, hDCMem, 0, 0, SRCCOPY);


	SelectObject(hDCMem, oldbmp);
	DeleteDC(hDCMem);
	DeleteObject(bitmap);

	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg) {
	case WM_KEYDOWN:
		keyStates[wp] = true;
		return 0;
	case WM_KEYUP:
		keyStates[wp] = false;
		return 0;
	case WM_CLOSE:		DestroyWindow(hWnd); return 0;
	case WM_DESTROY:	appEnd(); exit(0); return 0;
						//PostQuitMessage(0);  //May not be needed.
	case WM_PAINT:		PaintWindow(hWnd);   return 0;
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	#ifndef IS_CONSOLE_APPLICATION
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	#endif

	//appStart();

	WNDCLASSW wc = { 0 };
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	HWND hWnd = CreateWindowW(L"myWindowClass", 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, cWidth, cHeight, NULL, NULL, NULL, NULL);
	windowHandle = hWnd;

	//Initialize sound context
	ctx = cs_make_context(windowHandle, 48000, 8192*10, 0, NULL);

	appStart();
	cs_spawn_mix_thread(ctx);

	if (IsWindow(hWnd)) {
		DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		DWORD dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		HMENU menu = GetMenu(hWnd);
		RECT rc = { 0, 0, cWidth, cHeight };
		AdjustWindowRectEx(&rc, dwStyle, menu ? TRUE : FALSE, dwExStyle);
		SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
	}


	MSG msg = { 0 };
	//while (GetMessage(&msg, NULL, NULL, NULL)) {
	//while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
	clock_t timeOfLastFrame = 0;
	while (true) {
		if (clock() >= timeOfLastFrame + CLOCKS_PER_SEC / 60) {
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			UpdateWindow(hWnd);
			timeOfLastFrame = clock();
		}

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		for (size_t i = 0; i < activeSoundCount; i++) {
			if (activeSound[i]->active == false) {
				cs_free_sound(activeSound[i]->loaded_sound);
				free(activeSound[i]->loaded_sound);
				free(activeSound[i]);
				activeSoundCount--;
				activeSound[i] = activeSound[activeSoundCount];
			}
		}
		appLoop();
		if (activeSoundCount);
			//cs_mix(ctx);
	}

	return 0;
}

#ifdef IS_CONSOLE_APPLICATION
int main() {
	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}
#endif