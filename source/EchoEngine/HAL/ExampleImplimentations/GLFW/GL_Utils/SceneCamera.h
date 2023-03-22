#pragma once
#include <Vec.h>
#include <FixedPoint.h>

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
    SceneCamera(Vec3D<DECIMAL_TYPE> _position = { 0.0f, 0.0f, 0.0f }, Vec3D<DECIMAL_TYPE> _up = { 0, 1, 0 }) {//, FixedPoint<> _yaw = DEFAULT_YAW, FixedPoint<> _pitch = DEFAULT_PITCH) : front(Vec3D<FixedPoint<>>{0, 0, -1}), zoom(DEFAULT_ZOOM) {
        position = { _position.x, _position.y, _position.z };
        worldUp = { _up.x, _up.y, _up.z };
        //yaw = DEFAULT_YAW;
        //pitch = DEFAULT_PITCH;

        front = glm::vec3{ 0, 0, -1 };
        zoom = DEFAULT_ZOOM;
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void getFront(float& x, float& y, float& z) {
        x = front.x; y = front.y; z = front.z;
    }

    void setPosition(float x, float y, float z) {
        position = { x, y, z };
    }
    void setRotation(float x, float y, float z) {
        front = { x, y, z };

        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, front));
    }
};