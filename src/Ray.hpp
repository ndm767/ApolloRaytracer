#pragma once

#include <glm/glm.hpp>

#include "Scene.hpp"

class Ray {
  public:
    Ray(glm::vec3 origin, glm::vec3 direction);
    ~Ray();

    glm::vec3 traceRay(Scene &s);

  protected:
    glm::vec3 orig;
    glm::vec3 dir;
};