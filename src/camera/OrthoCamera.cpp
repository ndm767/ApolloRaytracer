#include "OrthoCamera.hpp"
#include "../Ray.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

OrthoCamera::OrthoCamera(int screenWidth, int screenHeight, glm::vec3 position,
                         glm::vec3 direction) {
    width = screenWidth;
    height = screenHeight;

    pos = position;
    origDir = glm::normalize(direction);
    dir = origDir;
    perpDir = glm::cross(dir, glm::vec3(0, 1, 0));
    perpDir = glm::normalize(perpDir);

    pitch = 0.0f;
    yaw = 0.0f;
}

OrthoCamera::~OrthoCamera() {}

Ray OrthoCamera::getRayAtPixel(int x, int y) {

    glm::vec3 orig = glm::vec3(0, 0, 0);
    orig.x = -1.0f + x * (2.0f / width);
    orig.y = 1.0f - y * (2.0f / height);

    orig += pos;

    if (origDir != glm::vec3(0, 1, 0) && origDir != glm::vec3(0, -1, 0)) {
        perpDir = glm::normalize(glm::cross(glm::vec3(0, -1, 0), dir));
    } else if (origDir == glm::vec3(0, -1, 0)) {
        perpDir = glm::normalize(glm::cross(glm::vec3(0, 0, -1), dir));
    } else {
        perpDir = glm::normalize(glm::cross(glm::vec3(0, 0, 1), dir));
    }

    Ray r(orig, glm::normalize(dir - orig), 3);
    return r;
}

void OrthoCamera::translate(glm::vec3 delta) { pos += delta; }

// rotate camera by certain number of degrees
void OrthoCamera::rotate(float deltaPitch, float deltaYaw) {
    pitch += glm::radians(deltaPitch);
    yaw += glm::radians(deltaYaw);

    constexpr float pi = 3.14159f;

    // limit pitch to 45 deg in each direction
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