#pragma once

#include <glm/glm.hpp>

// basic point light
class Light {
  public:
    Light(glm::vec3 position, float intensity,
          glm::vec3 color = glm::vec3(1, 1, 1));
    ~Light();

    glm::vec3 getPos() { return pos; }
    glm::vec3 getCol() { return col; }
    float getIntensity() { return inten; }

  protected:
    glm::vec3 pos;
    glm::vec3 col;
    float inten;
};