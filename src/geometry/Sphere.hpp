#pragma once

#include "Object.hpp"

class Sphere : public Object {
  public:
    Sphere(glm::vec3 center, float radius, Material mat);
    ~Sphere();

    bool testIntersection(HitData &data) override;

    glm::vec3 getCenter() { return pos; }
    float getRadius() { return r; }

  protected:
    float r;
};