#pragma once

#include "Object.hpp"

class Sphere : public Object {
  public:
    Sphere(glm::vec3 center, float radius, Material mat);
    ~Sphere();

    bool testIntersection(HitData &data) override;

  protected:
    float r;
};