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

  protected:
    int width, height;
    glm::vec3 pos;
    glm::vec3 dir;
};