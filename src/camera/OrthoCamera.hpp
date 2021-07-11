#include "Camera.hpp"

// orthographically-projected camera
class OrthoCamera : public Camera {
  public:
    OrthoCamera(int screenWidth, int screenHeight, glm::vec3 position,
                glm::vec3 direction);
    ~OrthoCamera();

    Ray getRayAtPixel(int x, int y) override;

    void translate(glm::vec3 delta) override;
    void rotate(float deltaPitch, float deltaYaw) override;
};