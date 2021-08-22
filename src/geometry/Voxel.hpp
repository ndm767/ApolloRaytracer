#pragma once

#include "Object.hpp"
#include "Triangle.hpp"

#include <memory>
#include <vector>

// basic bounding box to speed up mesh rendering
class Voxel : public Object {
  public:
    Voxel(glm::vec3 _corner, float _length, int _depth, int _maxDepth,
          std::vector<std::shared_ptr<Triangle>> tris, Material mat);
    ~Voxel();

    bool testIntersection(HitData &data) override;

  protected:
    bool didHitVoxel(glm::vec3 rayOrig, glm::vec3 rayDir);

    int depth, maxDepth;
    glm::vec3 corner;
    float length;
    std::vector<std::shared_ptr<Voxel>> subVoxels;
    std::vector<std::shared_ptr<Triangle>> containedTris;
};