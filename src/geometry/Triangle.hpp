#pragma once

#include "Object.hpp"

#include <array>

// triangle mesh
class Triangle : public Object {
  public:
    Triangle(glm::vec3 points[3], Material mat,
             glm::vec3 triNorm = glm::vec3(0.0f, 0.0f, 0.0f));
    ~Triangle();

    bool testIntersection(HitData &data) override;

    std::array<glm::vec3, 3> getPoints() {
        return std::array<glm::vec3, 3>{p[0], p[1], p[2]};
    }
    glm::vec3 getNorm() { return norm; }

  protected:
    glm::vec3 p[3];
    glm::vec3 edge1;
    glm::vec3 edge2;

    glm::vec3 norm;
};