#pragma once

#include "Object.hpp"

// triangle mesh
class Triangle : public Object {
  public:
    Triangle(glm::vec3 points[3], Material mat,
             glm::vec3 triNorm = glm::vec3(0.0f, 0.0f, 0.0f));
    ~Triangle();

    bool testIntersection(HitData &data) override;

  protected:
    glm::vec3 p[3];

    glm::vec3 norm;
};