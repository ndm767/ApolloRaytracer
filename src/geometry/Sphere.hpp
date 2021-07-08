#pragma once

#include "Object.hpp"

class Sphere : public Object {
  public:
    Sphere(glm::vec3 center, float radius);
    ~Sphere();

    bool testIntersection(glm::vec3 rayOrig, glm::vec3 rayDir) override;

  protected:
    float r;
};