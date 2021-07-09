#include "PerspCamera.hpp"
#include "../Ray.hpp"

#include <glm/gtx/rotate_vector.hpp>

PerspCamera::PerspCamera(int screenWidth, int screenHeight, glm::vec3 position,
                         glm::vec3 direction, float horizFOV) {
    width = screenWidth;
    height = screenHeight;

    pos = position;
    origDir = glm::normalize(direction);
    dir = origDir;

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

    glm::vec3 rotDir = origDir;
    rotDir = glm::rotate(rotDir, glm::radians(pitch), glm::vec3(1, 0, 0));
    rotDir = glm::rotate(rotDir, glm::radians(yaw), glm::vec3(0, 1, 0));

    // rotate lookPos to point towards where we actually want to look
    float angle = acos(glm::dot(glm::vec3(0, 0, 1), rotDir));
    if (angle != 0) {
        glm::vec3 axis = glm::cross(glm::vec3(0, 0, 1), rotDir);
        lookDir = glm::rotate(lookDir, angle, axis);
    }

    lookDir = glm::normalize(lookDir);

    Ray r(orig, lookDir);
    return r;
}

void PerspCamera::translate(glm::vec3 delta) { pos += delta; }

// rotate camera by certain number of degrees
void PerspCamera::rotate(float deltaPitch, float deltaYaw) {

    pitch += deltaPitch;
    yaw += deltaYaw;

    dir = origDir;
    dir = glm::rotate(dir, glm::radians(pitch), glm::vec3(1, 0, 0));
    dir = glm::rotate(dir, glm::radians(yaw), glm::vec3(0, 1, 0));

    if (yaw < 0) {
        yaw = 360 + yaw;
    } else if (yaw > 360) {
        yaw = yaw - 360;
    }

    if (pitch > 60.0f) {
        pitch = 60.0f;
    } else if (pitch < -60.0f) {
        pitch = -60.0f;
    }
}