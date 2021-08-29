#pragma once

#include "Camera.hpp"

// perspective camera
class PerspCamera : public Camera {
  public:
    PerspCamera(int screenWidth, int screenHeight, glm::vec3 position,
                glm::vec3 direction, float horizFOV, int _maxRayDepth = 3);
    ~PerspCamera();

    Ray getRayAtPixel(float x, float y) override;

    void translate(glm::vec3 delta) override;
    void rotate(float deltaPitch, float deltaYaw) override;

  protected:
    float hFOV;
    float vFOV;
};