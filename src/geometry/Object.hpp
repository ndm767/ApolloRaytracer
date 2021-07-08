#pragma once

#include "../HitData.hpp"
#include "../Material.hpp"

#include <glm/glm.hpp>

// object parent class
class Object {
  public:
    Object() {}
    virtual ~Object() {}

    // see if ray intersects with object
    virtual bool testIntersection(HitData &data) = 0;

    Material &getMaterial() { return objMat; }

  protected:
    glm::vec3 pos;
    Material objMat;
};