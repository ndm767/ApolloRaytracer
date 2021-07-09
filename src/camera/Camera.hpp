#pragma once

#include <glm/glm.hpp>

// forward declaration because camera references ray which references scene
// which references camera
class Ray;

// camera parent class
class Camera {
  public:
    Camera() {}
    virtual ~Camera() {}

    virtual Ray getRayAtPixel(int x, int y) = 0;

    virtual void translate(glm::vec3 delta) = 0;
    virtual void rotate(float deltaPitch, float deltaYaw) = 0;

    float getPitch() { return pitch; }
    float getYaw() { return yaw; }
    glm::vec3 getDir() { return dir; }

  protected:
    int width, height;
    glm::vec3 pos;
    glm::vec3 origDir;
    glm::vec3 dir;

    float pitch, yaw;
};