#pragma once

#include <glm/glm.hpp>

// object parent class
class Object {
  public:
    Object() {}
    virtual ~Object() {}

    // see if ray intersects with object
    virtual bool testIntersection(glm::vec3 rayOrig, glm::vec3 rayDir) = 0;

  protected:
    glm::vec3 pos;
};