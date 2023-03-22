#pragma once
#include "DArray.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Shapes.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct TexturedQuad {
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexBufferLayout vertexLayout;
    VertexArray vertexArray;
    Shader shader;
    Texture texture;
    float colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float pos[2];
    float siz[2];
    static constexpr Vertex2D verts[] = {
        /*{-0.5f,-0.5f, 0.0f, 0.0f},
        { 0.5f,-0.5f, 1.0f, 0.0f },
        { 0.5f, 0.5f, 1.0f, 1.0f },
        {-0.5f, 0.5f, 0.0f, 1.0f }*/
        {-0.5f, 0.5f, 0.0f, 0.0f},  //bottom left
        {0.5f,  0.5f, 1.0f, 0.0f},  //bottom right
        {0.5f, -0.5f, 1.0f, 1.0f},  //top right
        {-0.5f, -0.5f, 0.0f, 1.0f}   //top left
    };

    //void init(const Vertex2D* verticies, const uint8_t* indicies, uint32_t vertCount, uint32_t indexCount);
    void init(const char* filename, float x, float y, float w, float h);
    void destruct();
    void setColor(float r, float g, float b, float a);
    void draw();
    void draw(float x, float y, float w, float h);
    void setPosAndSiz(float x, float y, float w, float h);
};

/*void Billboard::init(const Vertex2D* verticies, const uint8_t* indicies, uint32_t vertCount, uint32_t indexCount) {
    vertexBuffer.init(verticies, vertCount * sizeof(Vertex2D));
    indexBuffer.init(indicies, indexCount, 2);
    vertexArray.init();
    vertexLayout.push<float>(2);
    vertexLayout.push<float>(2);
    vertexArray.addBuffer(vertexBuffer, vertexLayout);
    shader.init("./Shaders/Basic.shader");
    shader.setUniform4f("u_color", 0.0f, 0.0f, 1.0f, 1.0f);
    shader.setUniform1i("u_texture", 0);
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    shader.setUniformMat4f("u_MVP", proj);
    //shader.init("./Shaders/Basic.shader");
    texture.init("./data/textures/ChernoLogo.png");
}*/
void TexturedQuad::init(const char* filename, float x, float y, float w, float h) {
    std::string dataDir = EE_getDirData();
    shader.init(dataDir + "ShadersGL/TexturedQuad.shader");
    //texture.init(dataDir + "textures/ChernoLogo.png");
    texture.init(filename);
    uint8_t indicies[] = {
        0, 1, 2,
        2, 3, 0
    };
    Vertex2D newVerts[4];
    for (uint32_t i = 0; i < 4; i++) {
        newVerts[i].x = verts[i].x * w; newVerts[i].y = verts[i].y * h;
        newVerts[i].x = verts[i].x + w / 2; newVerts[i].y = verts[i].y + h / 2;
        newVerts[i].x += x; newVerts[i].y += y;
        translateScreen2DToGL(newVerts[i].x, newVerts[i].y);
    }
    vertexBuffer.init(newVerts, 4 * sizeof(Vertex2D));
    indexBuffer.init(indicies, 6, 1);
    vertexArray.init();
    vertexLayout.push<float>(2);
    vertexLayout.push<float>(2);
    vertexArray.addBuffer(vertexBuffer, vertexLayout);
}
void TexturedQuad::destruct() {
    vertexBuffer.destruct();
    indexBuffer.destruct();
    vertexArray.destruct();
    shader.destruct();
    texture.destruct();
}
void TexturedQuad::setColor(float r, float g, float b, float a) {
    colors[0] = r, colors[1] = g, colors[2] = b, colors[3] = a;
}
void TexturedQuad::draw() {
    vertexBuffer.bind();
    indexBuffer.bind();
    texture.bind();
    shader.setUniform4f("u_color", colors[0], colors[1], colors[2], colors[3]);
    shader.setUniform1i("u_texture", 0);
    //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    //float offsetX = 5, offsetY = 10;
    //translateScreen2DToGL(offsetX, offsetY);
    //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    //shader.setUniformMat4f("u_MVP", proj);
    shader.bind();
    vertexArray.bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_BYTE, nullptr);
}
void TexturedQuad::draw(float x, float y, float w, float h) {
    setPosAndSiz(x, y, w, h);
    draw();
}
void TexturedQuad::setPosAndSiz(float x, float y, float w, float h) {
    uint8_t indicies[] = {
        0, 1, 2,
        2, 3, 0
    };
    Vertex2D newVerts[4];
    for (uint32_t i = 0; i < 4; i++) {
        newVerts[i].u = verts[i].u; newVerts[i].v = verts[i].v;
        newVerts[i].x = verts[i].x * w; newVerts[i].y = verts[i].y * h;
        newVerts[i].x += verts[i].x + w / 2; newVerts[i].y += verts[i].y + h / 2;
        newVerts[i].x += x; newVerts[i].y += y;
        translateScreen2DToGL(newVerts[i].x, newVerts[i].y);
    }

    vertexBuffer.buffer(newVerts, 4 * sizeof(Vertex2D));
    indexBuffer.buffer(indicies, 6, 1);
    vertexBuffer.bind();
    indexBuffer.bind();
    vertexArray.addBuffer(vertexBuffer, vertexLayout);
}