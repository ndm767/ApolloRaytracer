#pragma once

#include "BoundingBox.hpp"
#include "Triangle.hpp"

#include <memory>
#include <string>
#include <vector>

class Mesh : public Object {
  public:
    Mesh(std::string path, glm::vec3 meshPos, float scale, Material mat);
    ~Mesh();

    bool testIntersection(HitData &data) override;

  protected:
    std::vector<std::shared_ptr<Triangle>> tris;
    std::shared_ptr<BoundingBox> bb;
};