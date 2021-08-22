#pragma once

#include "Object.hpp"

// basic bounding box to speed up mesh rendering
class BoundingBox : public Object {
  public:
    BoundingBox(glm::vec3 _minPos, glm::vec3 _maxPos);
    ~BoundingBox();

    glm::vec3 getMinPos() { return minPos; }
    glm::vec3 getMaxPos() { return maxPos; }

    bool testIntersection(HitData &data) override;

  protected:
    glm::vec3 minPos, maxPos;
};