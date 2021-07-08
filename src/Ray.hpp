#pragma once

#include <glm/glm.hpp>

#include "geometry/Object.hpp"

class Ray {
  public:
    Ray(glm::vec3 origin, glm::vec3 direction);
    ~Ray();

    glm::vec3 traceRay(Object *o);

  protected:
    glm::vec3 orig;
    glm::vec3 dir;
};