#pragma once

#include "Object.hpp"

class Sphere : public Object {
  public:
    Sphere(glm::vec3 center, float radius);
    ~Sphere();

    bool testIntersection(HitData &data) override;

  protected:
    float r;
};