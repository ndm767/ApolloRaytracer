#pragma once

#include <glm/glm.hpp>

#include "Scene.hpp"

class Ray {
  public:
    Ray(glm::vec3 origin, glm::vec3 direction);
    ~Ray();

    glm::vec3 traceRay(Scene &s);

    glm::vec3 getOrig() { return orig; }
    glm::vec3 getDir() { return dir; }

  protected:
    glm::vec3 orig;
    glm::vec3 dir;
};