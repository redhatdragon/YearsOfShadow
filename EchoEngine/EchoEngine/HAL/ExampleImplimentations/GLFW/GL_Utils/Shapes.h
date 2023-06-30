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

struct Vertex3D {
    float x, y, z, u, v;// , z;
    bool operator==(const Vertex2D& other) const {
        if (x != other.x || y != other.y || u != other.u || v != other.v)//|| z != other.z)
            return false;
        return true;
    }

    struct HashFunction {
        size_t operator()(const Vertex3D& pos) const {
            size_t rowHash = std::hash<float>()(pos.x);
            size_t colHash = std::hash<float>()(pos.y) << 1;
            size_t depHash = std::hash<float>()(pos.y) << 2;
            size_t uHash = std::hash<float>()(pos.u) << 3;
            size_t vHash = std::hash<float>()(pos.v) << 4;
            return rowHash ^ colHash ^ depHash * uHash ^ vHash;
        }
    };
};
struct Triangle3D {
    Vertex3D verticies[3];
};