#pragma once

#include "EchoEngine/Vec.h"
#include <math.h>

class FPSCamera {
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    //typedef FixedPoint<256 * 256> DECIMAL_TYPE;
    typedef float DECIMAL_TYPE;

    //static constexpr int DEFAULT_YAW = -90;  //TODO: needs changing...
    static constexpr int DEFAULT_YAW = 0;  //TODO: needs changing...
    static constexpr int DEFAULT_PITCH = 0;
    static constexpr int DEFAULT_ZOOM = 40;

    DECIMAL_TYPE mouseSensitivity;
    DECIMAL_TYPE movementSpeed;

    // camera Attributes
    Vec3D<DECIMAL_TYPE> position;
    Vec3D<DECIMAL_TYPE> front;
    Vec3D<DECIMAL_TYPE> up;
    Vec3D<DECIMAL_TYPE> right;
    Vec3D<DECIMAL_TYPE> worldUp;
    // euler Angles
    DECIMAL_TYPE yaw;
    DECIMAL_TYPE pitch;
    // camera options
    DECIMAL_TYPE zoom;
public:

    // constructor with vectors
    void init() {//, FixedPoint<> _yaw = DEFAULT_YAW, FixedPoint<> _pitch = DEFAULT_PITCH) : front(Vec3D<FixedPoint<>>{0, 0, -1}), zoom(DEFAULT_ZOOM) {
        position = { 0.0f, 0.0f, 0.0f };
        worldUp = { 0, -1, 0 };
        yaw = DEFAULT_YAW;
        pitch = DEFAULT_PITCH;

        //front = glm::vec3{ 0, 0, -1 };
        front = { 0, 0, 0 };
        zoom = DEFAULT_ZOOM;

        mouseSensitivity = 1.0f * 100;
        movementSpeed = 0.016f;

        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    /*glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }*/
    Vec3D<DECIMAL_TYPE> getWorldLookAtPos() {
        return front + position;
    }
    Vec3D<DECIMAL_TYPE> getLocalLookAtPos() {
        return front;
    }
    void setPosition(Vec3D<DECIMAL_TYPE> pos) {
        position = pos;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(DECIMAL_TYPE xoffset, DECIMAL_TYPE yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        uint32_t rotationCount = abs((int32_t)(yaw / 360));
        if (rotationCount)
            if(yaw > 0)
                yaw -= (DECIMAL_TYPE)(rotationCount * 360);
            else
                yaw += (DECIMAL_TYPE)(rotationCount * 360);
        //rotationCount = (uint32_t)(pitch / 360);
        //if (rotationCount)
        //    pitch -= (DECIMAL_TYPE)(rotationCount * 360);

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (pitch > 89)
                pitch = 89;
            if (pitch < -89)
                pitch = -89;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {

        //front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        //front.y = sin(glm::radians(pitch));
        //front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        //front = glm::normalize(front);
        //// also re-calculate the Right and Up vector
        //right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //up = glm::normalize(glm::cross(right, front));

        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front.normalize();
        // also re-calculate the Right and Up vector
        right = cross(front, worldUp);
        right.normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = cross(right, front);
        up.normalize();
    }

    float radians(float input) {
        return input * (3.141592f / 180);
    }
    Vec3D<DECIMAL_TYPE> cross(Vec3D<DECIMAL_TYPE> a, Vec3D<DECIMAL_TYPE> b) {
        Vec3D<DECIMAL_TYPE> retValue;

        retValue.x = a.y * b.z - a.z * b.y;
        retValue.y = -(a.x * b.z - a.z * b.x);
        retValue.z = a.x * b.y - a.y * b.x;
        return retValue;
    }
};