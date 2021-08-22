#pragma once

#include "BoundingBox.hpp"
#include "Triangle.hpp"

#include <memory>
#include <string>
#include <vector>

struct MeshInput {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned> indices;
};

class Mesh : public Object {
  public:
    Mesh(MeshInput vals, glm::vec3 meshPos, float scale, Material mat);
    ~Mesh();

    bool testIntersection(HitData &data) override;

    std::shared_ptr<BoundingBox> getBoundingBox() { return bb; }
    std::vector<std::shared_ptr<Triangle>> getTris() { return tris; }

  protected:
    std::vector<std::shared_ptr<Triangle>> tris;
    std::shared_ptr<BoundingBox> bb;
};