#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Shapes.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OBJ_Loader.h>

GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,   0.0f, 0.0f,
     1.0, -1.0,  1.0,   0.0f, 0.0f,
     1.0,  1.0,  1.0,   0.0f, 0.0f,
    -1.0,  1.0,  1.0,   0.0f, 0.0f,
    // top
    -1.0,  1.0,  1.0,   0.0f, 0.0f,
     1.0,  1.0,  1.0,   0.0f, 0.0f,
     1.0,  1.0, -1.0,   0.0f, 0.0f,
    -1.0,  1.0, -1.0,   0.0f, 0.0f,
    // back
     1.0, -1.0, -1.0,   0.0f, 0.0f,
    -1.0, -1.0, -1.0,   0.0f, 0.0f,
    -1.0,  1.0, -1.0,   0.0f, 0.0f,
     1.0,  1.0, -1.0,   0.0f, 0.0f,
     // bottom
     -1.0, -1.0, -1.0,  0.0f, 0.0f,
      1.0, -1.0, -1.0,  0.0f, 0.0f,
      1.0, -1.0,  1.0,  0.0f, 0.0f,
     -1.0, -1.0,  1.0,  0.0f, 0.0f,
     // left
     -1.0, -1.0, -1.0,  0.0f, 0.0f,
     -1.0, -1.0,  1.0,  0.0f, 0.0f,
     -1.0,  1.0,  1.0,  0.0f, 0.0f,
     -1.0,  1.0, -1.0,  0.0f, 0.0f,
     // right
      1.0, -1.0,  1.0,  0.0f, 0.0f,
      1.0, -1.0, -1.0,  0.0f, 0.0f,
      1.0,  1.0, -1.0,  0.0f, 0.0f,
      1.0,  1.0,  1.0,  0.0f, 0.0f
};
uint16_t cube_indices[24] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 11,
    12, 13, 14, 15,
    16, 17, 18, 19,
    20, 21, 22, 23
};
uint32_t cube_vertCount = 24;
uint32_t cube_indexCount = 24;

struct TEXTURE_ENUM {
    enum TYPE {
        DIFFUSE,
        BUMP
    };
};

struct SubMesh {
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexBufferLayout vertexLayout;
    VertexArray vertexArray;
    Shader* shader;
    std::vector<Texture> textures;
    //float colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    //float rot[2];  //ON HOLD

    SubMesh(const Vertex3D* verticies, const void* indices, uint32_t vertCount, uint32_t indexCount,
            const std::vector<Texture>& _textures) :
    vertexBuffer(verticies, vertCount * sizeof(Vertex3D)), indexBuffer(indices, indexCount, 4) {
        //vertexBuffer.init(verticies, vertCount * sizeof(Vertex3D));
        //indexBuffer.init(indices, indexCount, 4);
        
        //vertexBuffer.init(cube_vertices, cube_vertCount * sizeof(Vertex3D));
        //indexBuffer.init(cube_indices, cube_indexCount, 2);
        vertexArray.init();
        vertexLayout.push<float>(3); // Position
        vertexLayout.push<float>(2); // Texture Coords
        vertexLayout.push<float>(3); // Normals

        vertexArray.addBuffer(vertexBuffer, vertexLayout);

        // Shader
        {
            std::string shaderPath = HAL::get_dir_data();
            shaderPath += "ShadersGL/BasicMesh.shader";
            shader = ShaderCache::get(shaderPath);
        }

        // Texture
        {
            textures.push_back(Texture());
            std::string texturePath = HAL::get_dir_data();
            texturePath += "Textures/RightArrow.png";
            textures[0].init(texturePath);
        }
        
        //uint32_t textureCount = _textures.size();
        //for (uint32_t i = 0; i < textureCount; i++) {
        //    textures.push_back(_textures[i]);
        //}
        //textures[0].bind();
        //shader.setUniform1i("u_texture", 0);
        //shader.setUniform4f("u_color", 1, 1, 1, 1);
        //shader.setUniformMat4f("u_MVP", {});
    }
    void draw(Pos3D pos, Pos3D rot, Pos3D siz, glm::vec3 cam_pos, glm::vec3 cam_dir, const glm::mat4 &view,
              const glm::mat4 &perspective)
    {
        vertexBuffer.bind();
        indexBuffer.bind();
        //for(uint32_t i = 0; i < textures.size(); i++)
            //textures[i].bind(i);
        textures[0].bind();
        shader->bind();
        shader->setUniform4f("u_color", 1, 1, 1, 1);
        shader->setUniform1i("u_texture", 0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, { pos.x, pos.y, pos.z });
        model = glm::rotate(model, glm::radians(rot.x), { 1.0f, 0.0f, 0.0f });
        model = glm::rotate(model, glm::radians(rot.y), { 0.0f, 1.0f, 0.0f });
        model = glm::rotate(model, glm::radians(rot.z), { 0.0f, 0.0f, 1.0f });
        model = glm::scale(model, { siz.x, siz.y, siz.z });
        glm::mat4 mvp = perspective * view * model;
        shader->setUniformMat4f("u_M", model);
        shader->setUniformMat4f("u_MVP", mvp);
        shader->setUniform3f("u_camPos", cam_pos.x, cam_pos.y, cam_pos.z);
        // shader.setUniform3f("u_camDir", cam_dir.x, cam_dir.y, cam_dir.z);
        vertexArray.bind();

        vertexBuffer.bind();
        indexBuffer.bind();

        GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_SHORT, nullptr));

        //vertexBuffer.unbind();
        //indexBuffer.unbind();
        //textures[0].unbind();
        //shader.unbind();
        //vertexArray.unbind();
    }
};

struct Mesh {
    std::vector<SubMesh> subMeshes;
    Pos3D pos = { .5, .5, 0 };
    Pos3D rot = { 1, 1, 1 };
    Pos3D siz = { 1, 1, 1 };
    std::string filepath;

    Mesh(const char* _filepath);
    void draw(glm::vec3 cam_pos, glm::vec3 cam_dir, const glm::mat4 &viewMatrix, const glm::mat4 &perspectiveMatrix);

private:
    SubMesh loadSubMeshData(const objl::Mesh& mesh);
};

inline Mesh::Mesh(const char* _filepath) {
    //Vertex3D* vertices;
    //void* indices;
    //uint32_t vertCount;
    //uint32_t indexCount;

    pos = { .5, .5, 0 };
    rot = { 1, 1, 1 };
    siz = { 1, 1, 1 };
    filepath = _filepath;

    objl::Loader loader;
    loader.LoadFile(_filepath);
    auto& meshes = loader.LoadedMeshes;
    loader.LoadedIndices;
    //meshes[0].MeshMaterial;

    for (auto& mesh : meshes) {
        //subMeshes.push_back(SubMesh());
        //SubMesh* subMeshPtr = &subMeshes[subMeshes.size()-1];
        //std::vector<Vertex3D> vertices;
        //std::vector<void*> indices;
        //loadSubMeshData(mesh, vertices, indices);
        subMeshes.push_back(loadSubMeshData(mesh));
    }
    filepath = _filepath;
}
void Mesh::draw(glm::vec3 cam_pos, glm::vec3 cam_dir, const glm::mat4& viewMatrix, const glm::mat4& perspectiveMatrix) {
    for (uint32_t i = 0; i < subMeshes.size(); i++) {
        //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        subMeshes[i].draw(pos, rot, siz, cam_pos, cam_dir, viewMatrix, perspectiveMatrix);
    }
}
SubMesh Mesh::loadSubMeshData(const objl::Mesh& mesh) {
    std::vector<Vertex3D> vertices;
    uint32_t indexCount = mesh.Indices.size();
    uint32_t vertexCount = mesh.Vertices.size();
    
    std::vector<uint32_t> indices = mesh.Indices;
    vertices.reserve(vertexCount);

    for (uint32_t i = 0; i < vertexCount; i++) {
        Vertex3D toVert;
        auto& fromVert = mesh.Vertices[i];
        toVert.pos.x = fromVert.Position.X;
        toVert.pos.y = fromVert.Position.Y;
        toVert.pos.z = fromVert.Position.Z;
        toVert.texCoord.x = fromVert.TextureCoordinate.X;
        toVert.texCoord.y = fromVert.TextureCoordinate.Y;
        toVert.normal.x = fromVert.Normal.X;
        toVert.normal.y = fromVert.Normal.Y;
        toVert.normal.z = fromVert.Normal.Z;
        toVert.normal = glm::normalize(toVert.normal);
        vertices.push_back(toVert);
    }

    SubMesh retValue = { vertices.data(), indices.data(), vertexCount, indexCount, {}};
    return retValue;
}