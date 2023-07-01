#pragma once
#include "../../../../DArray.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Shapes.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OBJ-Loader/Source/OBJ_Loader.h>
#include <HAL/ExampleImplimentations/GLFW/GL_Utils/GL_Call.h>

#include "../../../../Vec.h"

struct InstancedSubMesh {
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexArray vertexArray;
    Shader shader;
    std::vector<Texture> textures;

    VertexBuffer positionsBuffer;

    void init(const Vertex3D* verticies, const void* indices, uint32_t vertCount, uint32_t indexCount,
        const std::vector<Texture>& _textures) {
        vertexBuffer.init(verticies, vertCount * sizeof(Vertex3D));
        if (vertCount <= 256 * 256)
            indexBuffer.init(indices, indexCount, 2);
        else
            indexBuffer.init(indices, indexCount, 4);
        vertexArray.init();
        VertexBufferLayout vertexLayout;
        vertexLayout.push<float>(3);
        vertexLayout.push<float>(2);
        vertexLayout.push<float>(3);
        vertexArray.addBuffer(vertexBuffer, vertexLayout);
        //std::string shaderPath = EE_getDirData(); shaderPath += "ShadersGL/BasicInstancedMesh.shader";
        //shader.init(shaderPath);
        textures.push_back(Texture());
        std::string texturePath = EE_getDirData(); texturePath += "Textures/RightArrow.png";
        textures[0].init(texturePath);

        positionsBuffer.init();
        std::vector<Vec3D<float>> positions = { {50, 50, 50} };
        setOffsets(positions);
        VertexBufferLayout positionsLayout;
        positionsLayout.push<float>(3, true);
        vertexArray.addBuffer(positionsBuffer, positionsLayout);

        std::string shaderPath = EE_getDirData(); shaderPath += "ShadersGL/BasicInstancedMesh.shader";
        shader.init(shaderPath);
    }
    void destruct() {
        vertexBuffer.destruct();
        indexBuffer.destruct();
        vertexArray.destruct();
        shader.destruct();
        for (auto i = 0; i < textures.size(); i++)
            textures[i].destruct();
    }
    void draw(Pos3D pos, Pos3D rot, Pos3D siz, const glm::mat4& view, const glm::mat4& perspective, uint32_t instanceCount) {
        vertexBuffer.bind();
        indexBuffer.bind();
        //positionsBuffer.bind();
        textures[0].bind();
        shader.bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, { pos.x, pos.y, pos.z });
        model = glm::rotate(model, glm::radians(rot.x), { 1.0f, 0.0f, 0.0f });
        model = glm::rotate(model, glm::radians(rot.y), { 0.0f, 1.0f, 0.0f });
        model = glm::rotate(model, glm::radians(rot.z), { 0.0f, 0.0f, 1.0f });
        model = glm::scale(model, { siz.x, siz.y, siz.z });
        glm::mat4 mvp = perspective * view * model;
        shader.setUniformMat4f("u_MVP", mvp);
        vertexArray.bind();

        GL_CALL(glDrawElementsInstanced(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_SHORT, nullptr, instanceCount));
    }

    void setOffsets(const std::vector<Vec3D<float>>& offsets) {
        //positionsBuffer.destruct();
        positionsBuffer.buffer(&offsets[0], offsets.size() * sizeof(Vec3D<float>));

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
    Pos3D pos = { .5, .5, 0 };
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
    void setOffsets(Vec3D<float>* _positions, uint32_t count) {
        std::vector<Vec3D<float>> offsets;
        offsets.resize(count);
        instanceCount = count;
        memcpy(&offsets[0], _positions, sizeof(Vec3D<float>) * count);
        for (InstancedSubMesh& subMesh : subMeshes) {
            subMesh.setOffsets(offsets);
        }
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