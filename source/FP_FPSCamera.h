#pragma once

#include "EchoEngine/FixedPoint.h"
#include "EchoEngine/Vec.h"
#include <math.h>

struct FPSCamera {
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    typedef FixedPoint<256 * 256> DECIMAL_TYPE;
    //typedef float DECIMAL_TYPE;

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

    // constructor with vectors
    void init() {//, FixedPoint<> _yaw = DEFAULT_YAW, FixedPoint<> _pitch = DEFAULT_PITCH) : front(Vec3D<FixedPoint<>>{0, 0, -1}), zoom(DEFAULT_ZOOM) {
        position = { 0, 0, 0 };
        //up = _up;
        worldUp = { 0, -1, 0 };
        yaw = DEFAULT_YAW;
        pitch = DEFAULT_PITCH;

        front = Vec3D<DECIMAL_TYPE>{ 0, 0, 0 };
        zoom = DEFAULT_ZOOM;

        mouseSensitivity = 100;
        movementSpeed = 1;

        updateCameraVectors();

        test();
    }

    Vec3D<DECIMAL_TYPE> getPosition() {
        return position;
    }
    void setPosition(const Vec3D<DECIMAL_TYPE>& pos) {
        position = pos;
    }

    Vec3D<float> getFrontAsFloat() {
        return { front.x.getAsFloat(), front.y.getAsFloat(), front.z.getAsFloat()};
    }
    Vec3D<DECIMAL_TYPE> getFront() {
        return front;
    }
    void setFront(const Vec3D<DECIMAL_TYPE>& direction) {
        front = direction;
    }

    Vec3D<DECIMAL_TYPE> getWorldLookAtPos() {
        Vec3D<DECIMAL_TYPE> atPos = front;
        //atPos += position;
        atPos.x = position.x + front.x;
        atPos.y = position.y + front.y;
        atPos.z = position.z + front.z;
        return atPos;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction) {
        DECIMAL_TYPE velocity = movementSpeed;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(DECIMAL_TYPE xoffset, DECIMAL_TYPE yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        //uint32_t rotationCount = (uint32_t)(yaw.getAsInt() / 360);
        //if (rotationCount)
        //    yaw -= (DECIMAL_TYPE)(rotationCount * 360);
        //rotationCount = (uint32_t)(pitch.getAsInt() / 360);
        //if (rotationCount)
        //    pitch -= (DECIMAL_TYPE)(rotationCount * 360);

        //uint32_t rotationCount = (uint32_t)(yaw.getAsInt() / 360);
        //yaw -= (DECIMAL_TYPE)(rotationCount * 360);
        //rotationCount = (uint32_t)(pitch.getAsInt() / 360);
        //pitch -= (DECIMAL_TYPE)(rotationCount * 360);

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (pitch.getAsInt() > 89)
                pitch = 89;
            if (pitch.getAsInt() < -89)
                pitch = -89;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(DECIMAL_TYPE yoffset) {
        zoom -= yoffset;
        if (zoom < DECIMAL_TYPE(1))
            zoom = DECIMAL_TYPE(1);
        if (zoom > DECIMAL_TYPE(45))
            zoom = DECIMAL_TYPE(45);
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        /*// calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));*/

        //front.x = cos(radians(yaw)) * cos(radians(pitch));
        //front.y = sin(radians(pitch));
        //front.z = sin(radians(yaw)) * cos(radians(pitch));
        //front = glm::normalize(front);
        //// also re-calculate the Right and Up vector
        //right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //up = glm::normalize(glm::cross(right, front));

        front.x = cosin16(fixedDegreesToRadians(yaw)) * cosin16(fixedDegreesToRadians(pitch));
        front.y = sin16(fixedDegreesToRadians(pitch));
        front.z = sin16(fixedDegreesToRadians(yaw)) * cosin16(fixedDegreesToRadians(pitch));
        front.normalize();
        right = fixedCross(front, worldUp);
        right.normalize();
        up = fixedCross(right, front);
        up.normalize();
    }

    DECIMAL_TYPE fixedDegreesToRadians(DECIMAL_TYPE in) {
        static DECIMAL_TYPE fixedPI = "3.141592f";
        DECIMAL_TYPE retValue = in * (fixedPI / 180);
        return retValue;
    }
    DECIMAL_TYPE fixedRadiansToDegrees(DECIMAL_TYPE in) {
        static DECIMAL_TYPE fixedPI = "3.141592f";
        DECIMAL_TYPE retValue = in * (DECIMAL_TYPE(180) / fixedPI);
        return retValue;
    }
    DECIMAL_TYPE fixedCos(DECIMAL_TYPE val, uint32_t terms = 10) {
        //return cos(val.getAsFloat());
        /*
        int div = (int)(x / CONST_PI);
        x = x - (div * CONST_PI);
        char sign = 1;
        if (div % 2 != 0)
            sign = -1;

        double result = 1.0;
        double inter = 1.0;
        double num = x * x;
        for (int i = 1; i <= y; i++)
        {
            double comp = 2.0 * i;
            double den = comp * (comp - 1.0);
            inter *= num / den;
            if (i % 2 == 0)
                result += inter;
            else
                result -= inter;
        }
        return sign * result;
        */  //Ref:  https://austinhenley.com/blog/cosine.html

        //static DECIMAL_TYPE fixedPI = "3.141592f";
        //int div = (val / fixedPI).getAsInt();
        //val = val - (fixedPI * div);
        //int8_t sign = 1;
        //if (div % 2 != 0)
        //    sign = -1;
        //
        //DECIMAL_TYPE result = "1.0f";
        //DECIMAL_TYPE inter = "1.0f";
        //DECIMAL_TYPE num = val * val;
        //for (unsigned int i = 1; i <= terms; i++)
        //{
        //    DECIMAL_TYPE comp = "2.0f"; comp = comp * i;
        //    DECIMAL_TYPE den = comp * (comp - "1.0");
        //    inter *= num / den;
        //    if (i % 2 == 0)
        //        result += inter;
        //    else
        //        result -= inter;
        //}
        //return result * sign;

        static DECIMAL_TYPE fixedPI = "3.141592f";
        while (val < 0) 
            val += fixedPI * 2;
        while (val > fixedPI * 2) 
            val -= fixedPI * 2;
        DECIMAL_TYPE t = 1;
        DECIMAL_TYPE cos = t;
        for (int a = 1; a < 40; ++a)
        {
            DECIMAL_TYPE mult = -val * val / ((2 * a) * (2 * a - 1));
            t *= mult;
            cos += t;
        }
        return cos;
    }

    DECIMAL_TYPE fixedSin(DECIMAL_TYPE val, uint32_t terms = 6) {
        //return sin(val.getAsFloat());
        /*
        int i = 1;
        double cur = x;
        double acc = 1;
        double fact = 1;
        double pow = x;
        while (fabs(acc) > .00000001 && i < 100) {
            fact *= ((2 * i) * (2 * i + 1));
            pow *= -1 * x * x;
            acc = pow / fact;
            cur += acc;
            i++;
        }
        return cur;
        */  //Ref: https://stackoverflow.com/questions/2284860/how-does-c-compute-sin-and-other-math-functions
        
        //DECIMAL_TYPE multiplier = 1;
        //if (val > 1 || val < -1)
        //    multiplier = val;
        
        //int i = 1;
        //DECIMAL_TYPE cur = val;
        //DECIMAL_TYPE acc = 1;
        //DECIMAL_TYPE fact = 1;
        //DECIMAL_TYPE pow = val;
        //while (acc.getABS() > ".00000001f" && i < terms) {
        //    fact *= (DECIMAL_TYPE)((2 * i) * (2 * i + 1));
        //    pow *= val * val * -1;
        //    acc = pow / fact;
        //    cur += acc;
        //    i++;
        //}
        //return cur;

        const DECIMAL_TYPE my_pi = "3.14159265358979323846f";
        val = fmod(val, my_pi * 2);
        if (val < 0) {
            val = my_pi * 2 - val;
        }
        int8_t sign = 1;
        if (val > my_pi) {
            val -= my_pi;
            sign = -1;
        }

        DECIMAL_TYPE result = val;
        DECIMAL_TYPE coefficent = 3;
        for (int i = 0; i < terms; i++) {
            DECIMAL_TYPE pow = power(val, coefficent);
            DECIMAL_TYPE frac = factorial(coefficent);

            if (frac != 0) {
                if (i % 2 == 0) {
                    result = result - (pow / frac);
                }
                else {
                    result = result + (pow / frac);
                }
            }
            coefficent = coefficent + 2;
        }

        return result * sign;
    }
    Vec3D<DECIMAL_TYPE> fixedCross(Vec3D<DECIMAL_TYPE> a, Vec3D<DECIMAL_TYPE> b) {
        //Ref: https://www.tutorialspoint.com/cplusplus-program-to-compute-cross-product-of-two-vectors
        //c_P[0] = v_A[1] * v_B[2] - v_A[2] * v_B[1];
        //c_P[1] = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
        //c_P[2] = v_A[0] * v_B[1] - v_A[1] * v_B[0];
        Vec3D<DECIMAL_TYPE> retValue;
        retValue.x = a.y * b.z - a.z * b.y;
        retValue.y = -(a.x * b.z - a.z * b.x);
        retValue.z = a.x * b.y - a.y * b.x;
        return retValue;
    }

    void test() {
        float resultF;
        DECIMAL_TYPE result;
        int seed = rand() % 10;
        seed = -1;
        resultF = sin(seed);
        result = fixedSin(seed);
        std::cout << "Float, Fixed" << std::endl;
        std::cout << "Sin: " << seed << std::endl;
        std::cout << resultF << ' ' << result.getAsFloat() << std::endl;
        //seed = rand() % 10;
        resultF = cos(seed);
        result = fixedCos(seed);
        std::cout << "Cosin: " << seed << std::endl;
        std::cout << resultF << ' ' << result.getAsFloat() << std::endl;
    }
};