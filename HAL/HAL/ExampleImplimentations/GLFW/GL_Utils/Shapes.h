#pragma once

struct Pos2D {
    float x, y;
};
struct Pos3D {
    float x, y, z;
};
struct Vertex2D {
    float x, y, u, v;// , z;
    bool operator==(const Vertex2D& other) const {
        if (x != other.x || y != other.y || u != other.u || v != other.v)//|| z != other.z)
            return false;
        return true;
    }
    void log() const {
        HAL_LOG("Vertex2D: x, y, u, v;\n  {}, {}, {}, {}\n", x, y, u, v);
    }

    struct HashFunction {
        size_t operator()(const Vertex2D& pos) const {
            size_t rowHash = std::hash<float>()(pos.x);
            size_t colHash = std::hash<float>()(pos.y) << 1;
            size_t uHash = std::hash<float>()(pos.u) << 2;
            size_t vHash = std::hash<float>()(pos.v) << 3;
            return rowHash ^ colHash ^ uHash ^ vHash;
        }
    };
};
struct Triangle2D {
    Vertex2D verticies[3];
};

struct Vertex3D
{
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

struct Triangle3D {
    Vertex3D verticies[3];
};