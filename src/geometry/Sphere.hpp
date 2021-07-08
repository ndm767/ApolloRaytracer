#pragma once

#include "Object.hpp"

class Sphere : public Object {
  public:
    Sphere(glm::vec3 center, float radius,
           glm::vec3 color = glm::vec3(1, 1, 1));
    ~Sphere();

    bool testIntersection(HitData &data) override;

  protected:
    float r;
};