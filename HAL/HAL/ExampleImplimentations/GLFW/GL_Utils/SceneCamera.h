#pragma once

class SceneCamera {

    typedef float DECIMAL_TYPE;

    static constexpr int DEFAULT_ZOOM = 40;

    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    //DECIMAL_TYPE yaw;
    //DECIMAL_TYPE pitch;
    // camera options
    DECIMAL_TYPE zoom;
public:

    // constructor with vectors
    SceneCamera() {//, FixedPoint<> _yaw = DEFAULT_YAW, FixedPoint<> _pitch = DEFAULT_PITCH) : front(Vec3D<FixedPoint<>>{0, 0, -1}), zoom(DEFAULT_ZOOM) {
        position = { 0.0f, 0.0f, 0.0f };
        worldUp = { 0, 1, 0 };
        //yaw = DEFAULT_YAW;
        //pitch = DEFAULT_PITCH;

        front = glm::vec3{ 0, 0, -1 };
        zoom = DEFAULT_ZOOM;
    }

    glm::mat4 GetViewMatrix() {
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
        return glm::lookAt(position, position + front, up);
    }

    void getFront(float& x, float& y, float& z) {
        x = front.x; y = front.y; z = front.z;
    }

    void setFront(float x, float y, float z) {
        front.x = x; front.y = y; front.z = z;
    }
    glm::vec3 getPosition() {
        return {position.x, position.y, position.z};
    }
    void setPosition(float x, float y, float z) {
        position = { x, y, z };
    }
};