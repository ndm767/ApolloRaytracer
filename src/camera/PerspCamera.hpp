#pragma once

#include "Camera.hpp"

// perspective camera
class PerspCamera : public Camera {
  public:
    PerspCamera(int screenWidth, int screenHeight, glm::vec3 position,
                glm::vec3 direction, float horizFOV);
    ~PerspCamera();

    Ray getRayAtPixel(int x, int y);

  protected:
    float hFOV;
    float vFOV;
};