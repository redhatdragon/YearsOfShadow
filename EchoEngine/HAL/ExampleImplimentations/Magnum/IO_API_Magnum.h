#pragma once
#include "../../IO_API_3D.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColorGL.h>

#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/StringView.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/ImageView.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <time.h>
#include <stdio.h>
#include <iostream>

using namespace Magnum;
#include "TexturedQuadShader.h"

#ifdef DIR_TO_DATA 
#error "DIR_TO_DATA already defined"
#else
#define DIR_TO_DATA "../data/"
#endif

float FPS;
float FPSLimit = 30;



class MyApplication : public Platform::Application {
public:
	struct {
		Shaders::VertexColorGL2D vertexColorGL2D;
	} shaders;
	explicit MyApplication(const Arguments& arguments);

	void translateScreen2DToGL(float& x, float& y) {
		using namespace Magnum::Math::Literals;
		Vector2 out;
		Vector2i winSize = windowSize();
		out = { x / winSize.x(), (y / winSize.y()) * -1 };
		out += {-1.0f, 1.0f};
		//out *= {1.0f, -1.0f};
		x = out.x(); y = out.y();
		Color3 c;
	}
private:
	void drawEvent() override;
} *app;

MyApplication::MyApplication(const Arguments& arguments) :
	Platform::Application{ arguments } {

	//Platform::Application{ arguments, Configuration{}
		//.setTitle("7DaysToCry") }

	using namespace Math::Literals;
	GL::Renderer::setClearColor(0x000000_rgbf);
	//GL::Renderer::setClearColor(0xa5c9ea_rgbf);
	Debug{} << "Hello! This application is running on"
		<< GL::Context::current().version() << "using"
		<< GL::Context::current().rendererString();
	app = this;
	IO_appStart();
	std::cout << windowSize().x() << ", " << windowSize().y() << std::endl;
}

void MyApplication::drawEvent() {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
	IO_appLoop();
	swapBuffers();
}
MAGNUM_APPLICATION_MAIN(MyApplication)



void drawTexture(void* texture, int x, int y, int w, int h) {
	using namespace Math::Literals;
	GL::Mesh _mesh;
	TexturedQuadShader _shader;
	GL::Texture2D _texture;

	struct QuadVertex {
		Vector2 position;
		Vector2 textureCoordinates;
	};
	const QuadVertex vertices[]{
		{{ 0.5f, -0.5f}, {1.0f, 0.0f}}, /* Bottom right */
		{{ 0.5f,  0.5f}, {1.0f, 1.0f}}, /* Top right */
		{{-0.5f, -0.5f}, {0.0f, 0.0f}}, /* Bottom left */
		{{-0.5f,  0.5f}, {0.0f, 1.0f}}  /* Top left */
	};
	const UnsignedInt indices[]{        /* 3--1 1 */
		0, 1, 2,                        /* | / /| */
		2, 1, 3                         /* |/ / | */
	};                                  /* 2 2--0 */

	_mesh.setCount(Containers::arraySize(indices))
		.addVertexBuffer(GL::Buffer{ vertices }, 0,
			TexturedQuadShader::Position{},
			TexturedQuadShader::TextureCoordinates{})
		.setIndexBuffer(GL::Buffer{ indices }, 0,
			GL::MeshIndexType::UnsignedInt);

	//TODO: Left off on textured rectangle tutorial
}
void recolorTexture(void* texture, uint8_t r, uint8_t g, uint8_t b) {
	
}
void* getTexture(const char* fileName) {
	return 0;
}
void releaseTexture(void* texture) {
	
}
void drawText(const char* str, int x, int y, unsigned int fontWidth) {
	
}
void drawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	
}
void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	
}
void drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	
}

bool getKeyState(char k) {
	return false;
}
void getMouseState(uint8_t* leftButton, uint8_t* middleButton, uint8_t* rightButton) {
	
}
void getMouseCanvasPos(int* x, int* y) {
	
}

void getCanvasSize(unsigned int* width, unsigned int* height) {
	
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







void IO_drawTriangle2DRGB(const IO_Point2Df points[3], IO_ColorR8G8B8 color) {

}
void IO_drawTriangle2DRGBVertex(const IO_Point2Df points[3], const IO_ColorR8G8B8 colors[3]) {
	using namespace Math::Literals;
	struct TriangleVertex {
		Vector2 pos;
		Color3 color;
	};
	TriangleVertex vertices[3];
	for (unsigned int i = 0; i < 3; i++) {
		IO_Point2Df p = points[i];
		app->translateScreen2DToGL(p.x, p.y);
		memcpy(&vertices[i].pos, &p, sizeof(IO_Point2Df));
		float r = (float)(colors[i].buffer[0]) / 255.0f;
		float g = (float)(colors[i].buffer[1]) / 255.0f;
		float b = (float)(colors[i].buffer[2]) / 255.0f;
		vertices[i].color = { r, g, b };
	}
	GL::Mesh _mesh;
	_mesh.setCount(Containers::arraySize(vertices))
		.addVertexBuffer(GL::Buffer{ vertices }, 0,
			Shaders::VertexColorGL2D::Position{},
			Shaders::VertexColorGL2D::Color3{});
	_mesh.draw(app->shaders.vertexColorGL2D);
	//vertexColorGL2D.draw(_mesh);
}