#include "OctreeMesh.hpp"

OctreeMesh::OctreeMesh(Mesh &inputMesh, int numSubDiv) {
    subDiv = numSubDiv;
    objMat = inputMesh.getMaterial();

    auto meshBB = inputMesh.getBoundingBox();

    glm::vec3 c1 = meshBB->getMinPos();
    glm::vec3 c2 = meshBB->getMaxPos();

    float voxLen =
        std::max(std::max(std::abs(c2.x - c1.x), std::abs(c2.y - c1.y)),
                 std::abs(c2.z - c1.z));

    glm::vec3 voxCorner;
    voxCorner.x = std::min(c1.x, c2.x);
    voxCorner.y = std::min(c1.y, c2.y);
    voxCorner.z = std::min(c1.z, c2.z);

    baseVoxel =
        new Voxel(voxCorner, voxLen, 0, numSubDiv, inputMesh.getTris(), objMat);
}

OctreeMesh::~OctreeMesh() { delete baseVoxel; }

bool OctreeMesh::testIntersection(HitData &data) {

    HitData retData(data.getRayOrig(), data.getRayDir());
    retData.setObjDistSq(INFINITY);

    bool hit = baseVoxel->testIntersection(retData);

    if (hit) {
        data = retData;
        data.setHitObj((void *)this);
    }

    return hit;
}