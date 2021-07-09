#pragma once

#include "Object.hpp"

// triangle mesh
class Triangle : public Object {
  public:
    Triangle(glm::vec3 points[3], Material mat);
    ~Triangle();

    bool testIntersection(HitData &data) override;

  protected:
    glm::vec3 p[3];

    glm::vec3 norm;
};