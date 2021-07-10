#include "PerspCamera.hpp"
#include "../Ray.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

PerspCamera::PerspCamera(int screenWidth, int screenHeight, glm::vec3 position,
                         glm::vec3 direction, float horizFOV) {
    width = screenWidth;
    height = screenHeight;

    pos = position;
    origDir = glm::normalize(direction);
    dir = origDir;
    perpDir = glm::cross(dir, glm::vec3(0, 1, 0));
    perpDir = glm::normalize(perpDir);

    // horizontal FOV
    hFOV = glm::radians(horizFOV);
    // calculate vertical FOV from horizontal FOV
    vFOV = 2 * atan(tan(hFOV / 2) * height / width);

    pitch = 0.0f;
    yaw = 0.0f;
}

PerspCamera::~PerspCamera() {}

Ray PerspCamera::getRayAtPixel(int x, int y) {
    glm::vec3 orig = pos;

    // start with camera looking forward and find the pixel where the ray should
    // go through
    glm::vec3 lookDir = glm::vec3(0, 0, 1);
    lookDir.x = -1 * sin(hFOV) + x * (2 * sin(hFOV) / width);
    lookDir.y = sin(vFOV) - y * (2 * sin(vFOV) / height);

    // rotate lookDir to align with the original direction
    float angle = acos(glm::dot(glm::vec3(0, 0, 1), origDir));

    const float pi = acos(glm::dot(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)));

    if (angle != 0 && angle != pi) {
        glm::vec3 axis = glm::cross(glm::vec3(0, 0, 1), origDir);
        lookDir = glm::rotate(lookDir, angle, axis);
    } else if (angle == pi) {
        // handle if camera is pointed in z direction
        lookDir.x *= -1.0f;
        lookDir.z *= -1.0f;
    }

    // rotate lookDir to point towards where we actually want to look
    glm::vec3 pitchAxis = glm::cross(glm::vec3(0, 1, 0), origDir);

    // handle if camera is pointed in y direction
    if (pitchAxis == glm::vec3(0, 0, 0)) {
        pitchAxis = glm::vec3(1, 0, 0);
    }

    glm::vec3 yawAxis;
    if (origDir.z < 0) {
        yawAxis = glm::cross(glm::vec3(1, 0, 0), origDir);
    } else {
        yawAxis = glm::cross(glm::vec3(-1, 0, 0), origDir);
    }
    // handle if camera is pointed in x direction
    if (yawAxis == glm::vec3(0, 0, 0)) {
        yawAxis = glm::vec3(0, 1, 0);
    }
    lookDir = glm::rotate(lookDir, pitch, pitchAxis);
    lookDir = glm::rotate(lookDir, yaw, yawAxis);

    lookDir = glm::normalize(lookDir);

    if (origDir != glm::vec3(0, 1, 0) && origDir != glm::vec3(0, -1, 0)) {
        perpDir = glm::normalize(glm::cross(glm::vec3(0, -1, 0), dir));
    } else if (origDir == glm::vec3(0, -1, 0)) {
        perpDir = glm::normalize(glm::cross(glm::vec3(0, 0, -1), dir));
    } else {
        perpDir = glm::normalize(glm::cross(glm::vec3(0, 0, 1), dir));
    }

    Ray r(orig, lookDir);
    return r;
}

void PerspCamera::translate(glm::vec3 delta) { pos += delta; }

// rotate camera by certain number of degrees
void PerspCamera::rotate(float deltaPitch, float deltaYaw) {

    pitch += glm::radians(deltaPitch);
    yaw += glm::radians(deltaYaw);

    constexpr float pi = 3.14159f;

    // limit pitch to 45 degrees in each direction
    if (pitch > pi / 4.0f) {
        pitch = pi / 4.0f;
    } else if (pitch < -pi / 4.0f) {
        pitch = -pi / 4.0f;
    }

    dir = origDir;

    glm::vec3 pitchAxis = glm::cross(glm::vec3(0, 1, 0), origDir);
    if (pitchAxis == glm::vec3(0, 0, 0)) {
        pitchAxis = glm::vec3(1, 0, 0);
    }

    glm::vec3 yawAxis;
    if (origDir.z < 0) {
        yawAxis = glm::cross(glm::vec3(1, 0, 0), origDir);
    } else {
        yawAxis = glm::cross(glm::vec3(-1, 0, 0), origDir);
    }
    if (yawAxis == glm::vec3(0, 0, 0)) {
        yawAxis = glm::vec3(0, 1, 0);
    }

    dir = glm::rotate(dir, pitch, pitchAxis);
    dir = glm::rotate(dir, yaw, yawAxis);
    dir = glm::normalize(dir);
}