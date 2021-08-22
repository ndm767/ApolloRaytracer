#pragma once

#include "Mesh.hpp"
#include "Voxel.hpp"

#include <memory>
#include <vector>

class OctreeMesh : public Object {
  public:
    OctreeMesh(Mesh &inputMesh, int numSubDiv);
    ~OctreeMesh();

    bool testIntersection(HitData &data) override;

  protected:
    int subDiv;
    Voxel *baseVoxel;
};