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

struct TexturedQuad {
    //VertexBuffer vertexBuffer;
    //IndexBuffer indexBuffer;
    //VertexBufferLayout vertexLayout;
    //VertexArray vertexArray;

    uint32_t VBO, VAO, IBO;

    Shader shader;
    Texture texture;
    //uint32_t texture;
    float colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float pos[2];
    float siz[2];
    static constexpr Vertex2D verts[] = {
        /*{-0.5f,-0.5f, 0.0f, 0.0f},
        { 0.5f,-0.5f, 1.0f, 0.0f },
        { 0.5f, 0.5f, 1.0f, 1.0f },
        {-0.5f, 0.5f, 0.0f, 1.0f }*/
    
        //{-0.5f, 0.5f, 0.0f, 0.0f},  //bottom left
        //{0.5f,  0.5f, 1.0f, 0.0f},  //bottom right
        //{0.5f, -0.5f, 1.0f, 1.0f},  //top right
        //{-0.5f, -0.5f, 0.0f, 1.0f}  //top left
    
        //x, y, u, v
        //{0.5f, 0.5f, 1.0f, 1.0f},  //top right
        //{0.5f, -0.5f, 1.0f, 0.0f}, //bottom right
        //{-0.5f, -0.5f, 0.0f, 0.0f},//bottom left
        //{-0.5f,  0.5f, 0.0f, 1.0f} //top left
    
        {1, 1, 1.0f, 1.0f},  //top right
        {1, -1, 1.0f, 0.0f}, //bottom right
        {-1, -1, 0.0f, 0.0f},//bottom left
        {-1,  1, 0.0f, 1.0f} //top left
    
        //{1+1, 1+1, 1.0f, 1.0f},  //top right
        //{1+1, -1+1, 1.0f, 0.0f}, //bottom right
        //{-1+1, -1+1, 0.0f, 0.0f},//bottom left
        //{-1+1,  1+1, 0.0f, 1.0f} //top left

        //{1, 1, 1.0f, 1.0f},  //top right
        //{1, 0, 1.0f, 0.0f}, //bottom right
        //{0, 0, 0.0f, 0.0f},//bottom left
        //{0, 1, 0.0f, 1.0f} //top left
    };
    //static constexpr uint8_t indices[] = {
    //    0, 1, 2,
    //    2, 3, 0
    //};
    static constexpr uint8_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //void init(const Vertex2D* verticies, const uint8_t* indicies, uint32_t vertCount, uint32_t indexCount);
    void init(const char* filename, float x, float y, float w, float h);
    void destruct();
    void setColor(float r, float g, float b, float a);
    void draw();
    void draw(float x, float y, float w, float h);
    void setPosAndSiz(float x, float y, float w, float h);
private:
    void initTexture(const char* filename);
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
void TexturedQuad::init(const char* filename, float _x, float _y, float _w, float _h) {
    //Vertex2D origVertsTrans[4];
    //for (uint32_t i = 0; i < 4; i++) {
    //    origVertsTrans[i] = verts[i];
    //    translateGLToScreen2D();
    //}

    Vertex2D newVerts[4];
    //for (uint32_t i = 0; i < 4; i++) {
    //    newVerts[i].x = verts[i].x * w; newVerts[i].y = verts[i].y * h;
    //    newVerts[i].x = verts[i].x + w / 2; newVerts[i].y = verts[i].y + h / 2;
    //    newVerts[i].x += x; newVerts[i].y += y;
    //    //newVerts[i] = verts[i];
    //    translateScreen2DToGL(newVerts[i].x, newVerts[i].y);
    //    //newVerts[i] = verts[i];
    //    newVerts[i].u = verts[i].u;
    //    newVerts[i].v = verts[i].v;
    //    newVerts[i].log();
    //}

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);
    //vertexArray.init();
    //vertexBuffer.init();
    //indexBuffer.init(indices, 6, 1);
    //vertexArray.bind();

    setPosAndSiz(_x, _y, _w, _h);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //vertexLayout.push<float>(2);  //position
    //vertexLayout.push<float>(2);  //texture cord
    //vertexArray.addBuffer(vertexBuffer, vertexLayout);

    initTexture(filename);
    std::string dataDir = EE_getDirData();
    shader.init(dataDir + "ShadersGL/TexturedQuad.shader");
}
void TexturedQuad::destruct() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    //vertexArray.destruct();
    //vertexBuffer.destruct();
    //indexBuffer.destruct();
    shader.destruct();
    texture.destruct();
}
void TexturedQuad::setColor(float r, float g, float b, float a) {
    colors[0] = r, colors[1] = g, colors[2] = b, colors[3] = a;
}
void TexturedQuad::draw() {
    GL_CALL(glBindVertexArray(VAO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    //vertexArray.bind();
    //vertexBuffer.bind();
    //indexBuffer.bind();
    // bind textures on corresponding texture units
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texture);
    texture.bind();
    shader.bind();
    shader.setUniform4f("u_color", colors[0], colors[1], colors[2], colors[3]);
    shader.setUniform1i("u_texture", 0);

    glBindVertexArray(VAO);
    //vertexArray.bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void TexturedQuad::draw(float x, float y, float w, float h) {
    setPosAndSiz(x, y, w, h);
    draw();
}
void TexturedQuad::setPosAndSiz(float _x, float _y, float _w, float _h) {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(newVerts), newVerts, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //vertexBuffer.buffer(verts, sizeof(verts));
}
void TexturedQuad::initTexture(const char* filepath) {
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //// set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load image, create texture and generate mipmaps
    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);

    texture.init(filepath);
}