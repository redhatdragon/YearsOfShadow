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
#include <OBJ_Loader/OBJ_Loader.h>

struct InstancedSubMesh {
    Shader shader;
    std::vector<Texture> textures;

    GLuint VBO, IBO, PBO, VAO;

    uint32_t indexCount;

    //void* indicesCpy;

    void init(const Vertex3D* verticies, const void* indices, uint32_t vertCount, uint32_t _indexCount,
        const std::vector<Texture>& _textures) {
        //indicesCpy = malloc(_indexCount * 2);
        //memcpy(indicesCpy, indices, _indexCount * 2);

        indexCount = _indexCount;

        textures.push_back(Texture());
        std::string texturePath = EE_getDirData(); texturePath += "Textures/RightArrow.png";
        textures[0].init(texturePath);

        

        std::string shaderPath = EE_getDirData(); shaderPath += "ShadersGL/BasicInstancedMesh.shader";
        shader.init(shaderPath);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*vertCount, verticies, GL_DYNAMIC_DRAW);
        VertexBuffer;

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * 2, indices, GL_DYNAMIC_DRAW);
        IndexBuffer;

        glGenBuffers(1, &PBO);
        glBindBuffer(GL_ARRAY_BUFFER, PBO);
        VertexBuffer;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3*sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, PBO);
        glEnableVertexAttribArray(2);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)(5*sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)(0));
        glVertexAttribDivisor(2, 1);
        VertexArray;

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);
    }
    void destruct() {

    }
    void draw(Pos3D pos, Pos3D rot, Pos3D siz, const glm::mat4& view, const glm::mat4& perspective, uint32_t instanceCount) {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        textures[0].bind();
        shader.bind();
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, { pos.x, pos.y, pos.z });
        //model = glm::rotate(model, glm::radians(rot.x), { 1.0f, 0.0f, 0.0f });
        //model = glm::rotate(model, glm::radians(rot.y), { 0.0f, 1.0f, 0.0f });
        //model = glm::rotate(model, glm::radians(rot.z), { 0.0f, 0.0f, 1.0f });
        ////model = glm::scale(model, { siz.x, siz.y, siz.z });
        //model = glm::scale(model, { 1, 1, 1 });
        glm::mat4 mvp = perspective * view * model;
        shader.setUniformMat4f("u_MVP", mvp);
        shader.setUniform3f("u_scale", siz.x, siz.y, siz.z);
        shader.setUniform4f("u_color", 1, 1, 1, 1);

        glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr, instanceCount);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void setOffsets(const std::vector<glm::vec3>& offsets) {
        glBindBuffer(GL_ARRAY_BUFFER, PBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offsets.size(), &offsets[0], GL_DYNAMIC_DRAW);

        //positionsBuffer.destruct();

        //positionsBuffer.buffer(&offsets[0], offsets.size() * sizeof(Vec3D<float>));

        //shader.bind();
        ////shader.setUniform4f("u_color", 1, 1, 1, 1);
        //shader.setUniform1i("u_texture", 0);
        //for (uint32_t i = 0; i < offsets.size(); i++) {
        //    shader.setUniform3f(("offsets[" + std::to_string(i) + "]"), offsets[i].x, offsets[i].y, offsets[i].z);
        //}
    }
};

struct InstancedMesh {
    std::vector<InstancedSubMesh> subMeshes;
    Pos3D pos = { .5, -.5, -.5 };
    Pos3D rot = { 1, 1, 1 };
    Pos3D siz = { 1, 1, 1 };
    //std::vector<Vec3D<float>> offsets;
    uint32_t instanceCount;

    void init(const char* filepath) {
        objl::Loader loader;
        loader.LoadFile(filepath);
        auto& meshes = loader.LoadedMeshes;
        loader.LoadedIndices;
        for (uint32_t i = 0; i < meshes.size(); i++) {
            subMeshes.push_back(loadSubMeshData(meshes[i]));
        }
    }
    void destruct() {
        for (uint32_t i = 0; i < subMeshes.size(); i++)
            subMeshes[i].destruct();
    }
    void draw(const glm::mat4& viewMatrix, const glm::mat4& perspectiveMatrix) {
        for (uint32_t i = 0; i < subMeshes.size(); i++)
            subMeshes[i].draw(pos, rot, siz, viewMatrix, perspectiveMatrix, instanceCount);
    }
    void setOffsets(glm::vec3* _positions, uint32_t count)
    {
        if (count == 0)
            return;
        std::vector<glm::vec3> offsets;
        offsets.resize(count);
        instanceCount = count;
        memcpy(&offsets[0], _positions, sizeof(glm::vec3) * count);
        for (InstancedSubMesh& subMesh : subMeshes) {
            subMesh.setOffsets(offsets);
        }
    }
    void setScale(float sx, float sy, float sz) {
        siz = { sx, sy, sz };
    }
private:
    InstancedSubMesh loadSubMeshData(const objl::Mesh& mesh) {
        std::vector<Vertex3D> vertices;
        void* indices;
        uint32_t indexCount = mesh.Indices.size();
        uint32_t vertexCount = mesh.Vertices.size();
        InstancedSubMesh retValue;
        if (indexCount <= 256 * 256) {
            indices = malloc(indexCount * sizeof(uint16_t));
            uint16_t* indicesAs16 = (uint16_t*)indices;
            for (uint32_t i = 0; i < indexCount; i++) {
                indicesAs16[i] = (uint16_t)(mesh.Indices[i]);
            }
        }
        else {
            indices = malloc(indexCount * sizeof(uint32_t));
            uint32_t* indicesAs32 = (uint32_t*)indices;
            for (uint32_t i = 0; i < indexCount; i++) {
                indicesAs32[i] = (uint32_t)(mesh.Indices[i]);
            }
        }
        for (uint32_t i = 0; i < vertexCount; i++) {
            Vertex3D toVert;
            auto& fromVert = mesh.Vertices[i];
            toVert.x = fromVert.Position.X;
            toVert.y = fromVert.Position.Y;
            toVert.z = fromVert.Position.Z;
            toVert.u = fromVert.TextureCoordinate.X;
            toVert.v = fromVert.TextureCoordinate.Y;
            vertices.push_back(toVert);
        }

        retValue.init(vertices.data(), indices, vertexCount, indexCount, {});
        free(indices);
        return retValue;
    }
};