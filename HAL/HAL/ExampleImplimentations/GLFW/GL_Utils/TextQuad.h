#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Shapes.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct TextQuad {
    uint32_t VBO, VAO, IBO;
    Shader shader;
    Texture texture;
    //uint32_t texture;
    float colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float pos[2];
    float siz[2];
    static constexpr Vertex2D verts[] = {
        {1, 1, 1.0f, 1.0f},  //top right
        {1, -1, 1.0f, 0.0f}, //bottom right
        {-1, -1, 0.0f, 0.0f},//bottom left
        {-1,  1, 0.0f, 1.0f} //top left
    };
    static constexpr uint8_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    void init(Texture* fontAtlas, const char* str, float x, float y, float w, float h);
    void destruct();
    void setColor(float r, float g, float b, float a);
    void draw();
    void draw(float x, float y, float w, float h);
    void setPosAndSiz(float x, float y, float w, float h);
private:
    void initTexture(Texture* fontAtlas, const char* str);
};
void TextQuad::init(Texture* fontAtlas, const char* str, float _x, float _y, float _w, float _h) {
    Vertex2D newVerts[4];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);
    setPosAndSiz(_x, _y, _w, _h);
    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    initTexture(fontAtlas, str);
    std::string dataDir = EE_getDirData();
    shader.init(dataDir + "ShadersGL/TexturedQuad.shader");
}
void TextQuad::destruct() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    shader.destruct();
    texture.destruct();
}
void TextQuad::setColor(float r, float g, float b, float a) {
    colors[0] = r, colors[1] = g, colors[2] = b, colors[3] = a;
}
void TextQuad::draw() {
    GL_CALL(glBindVertexArray(VAO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    texture.bind();
    shader.bind();
    shader.setUniform4f("u_color", colors[0], colors[1], colors[2], colors[3]);
    shader.setUniform1i("u_texture", 0);

    glBindVertexArray(VAO);
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void TextQuad::draw(float x, float y, float w, float h) {
    setPosAndSiz(x, y, w, h);
    draw();
}
void TextQuad::setPosAndSiz(float _x, float _y, float _w, float _h) {
    float x = _x, y = _y, w = _w, h = _h;
    auto cSize = HAL::get_canvas_size();
    w = w / cSize.x; h = h / cSize.y;
    x = x / cSize.x; y = y / cSize.y;
    Vertex2D newVerts[4];
    for (uint32_t i = 0; i < 4; i++) {
        newVerts[i] = verts[i];
        newVerts[i].x = verts[i].x * w;
        newVerts[i].y = verts[i].y * h;
    }
    float glX = _x, glY = _y;
    glX = x - 0.5f; glY = y - 0.5f;
    float offX = glX * 2;
    float offY = glY * 2;
    offX += w;
    offY += h;
    for (uint32_t i = 0; i < 4; i++) {
        newVerts[i].x += offX;
        newVerts[i].y -= offY;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newVerts), newVerts, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
}
void TextQuad::initTexture(Texture* fontAtlas, const char* str) {
    uint8_t bytesPerPixel = fontAtlas->getBPP();
    uint32_t w = fontAtlas->getWidth();
    uint32_t widthPerChar = 8;  //TODO: make this dynamic or something later
    uint32_t h = fontAtlas->getHeight();
    const uint8_t* buff = fontAtlas->getPixels();
    uint32_t strLen = (uint32_t)strlen(str);
    uint32_t bytesPerCharLineSeg = widthPerChar * bytesPerPixel;
    uint8_t* data;
    HAL_ALLOC_RAWBYTE(data, strLen * bytesPerCharLineSeg * h);
    struct Pixel {
        uint8_t r, g, b, a;
        void log() {
            HAL_LOG("{}, {}, {}, {}\n", r, g, b, a);
        }
    };
    for (uint32_t i = 0; i < strLen; i++) {
        //HAL_LOG("START\n");
        for (uint32_t j = 0; j < h; j++) {
            memcpy(&data[i*bytesPerCharLineSeg + j*(strLen*bytesPerCharLineSeg)],
                &buff[(str[i]-' ')*bytesPerCharLineSeg + j*w*bytesPerPixel], bytesPerCharLineSeg);
            Pixel p;
            uint8_t* pPtr = (uint8_t*)&p;
            //HAL_LOG("ROW\n");
            for (uint32_t k = 0; k < widthPerChar; k++) {
                memcpy(pPtr,
                    &buff[((str[i]-' ')*bytesPerCharLineSeg + k*sizeof(Pixel)) + j*w*bytesPerPixel], sizeof(Pixel));
                //p.log();
            }
        }
    }
    texture.init(data, fontAtlas->getBPP(), strLen * widthPerChar, h);
    free(data);
}