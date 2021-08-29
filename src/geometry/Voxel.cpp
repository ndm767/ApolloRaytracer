#include "Voxel.hpp"

#include <algorithm>
#include <utility>

Voxel::Voxel(glm::vec3 _corner, float _length, int _depth, int _maxDepth,
             std::vector<std::shared_ptr<Triangle>> tris, Material mat) {
    objMat = mat;
    corner = _corner;
    length = _length;
    containedTris = tris;
    depth = _depth;
    maxDepth = _maxDepth;

    // divide voxels into 8 voxels
    // see which triangles go into which voxel
    // if a voxel is empty, do not add it to the subvoxels
    // repeat recursively until at max depth

    // generate subvoxels in for-loop hell
    if (depth < maxDepth) {
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                for (int z = 0; z < 2; z++) {
                    glm::vec3 newCorner = corner;
                    float lenStep = length / 2.0f;
                    newCorner.x += lenStep * x;
                    newCorner.y += lenStep * y;
                    newCorner.z += lenStep * z;

                    std::vector<std::shared_ptr<Triangle>> trisInSub;

                    for (auto t : containedTris) {
                        auto p = t->getPoints();

                        bool tX = false;
                        bool tY = false;
                        bool tZ = false;

                        float maxX = std::max(std::max(p[0].x, p[1].x), p[2].x);
                        float maxY = std::max(std::max(p[0].y, p[1].y), p[2].y);
                        float maxZ = std::max(std::max(p[0].z, p[1].z), p[2].z);

                        float minX = std::min(std::min(p[0].x, p[1].x), p[2].x);
                        float minY = std::min(std::min(p[0].y, p[1].y), p[2].y);
                        float minZ = std::min(std::min(p[0].z, p[1].z), p[2].z);

                        if (maxX >= newCorner.x &&
                            minX <= newCorner.x + lenStep) {
                            tX = true;
                        }
                        if (maxY >= newCorner.y &&
                            minY <= newCorner.y + lenStep) {
                            tY = true;
                        }
                        if (maxZ >= newCorner.z &&
                            minZ <= newCorner.z + lenStep) {
                            tZ = true;
                        }
                        if (tX && tY && tZ) {
                            trisInSub.push_back(t);
                        }
                    }

                    if (trisInSub.size() > 0) {
                        subVoxels.push_back(std::make_shared<Voxel>(
                            newCorner, lenStep, depth + 1, maxDepth, trisInSub,
                            objMat));
                    }
                }
            }
        }

        // stop storing contained triangles for non-terminal voxel
        containedTris.clear();
    }
}

Voxel::~Voxel() {}

bool Voxel::didHitVoxel(glm::vec3 rayOrig, glm::vec3 rayDir, float &hitT) {

    if (rayDir.x != 0) {
        if (rayDir.x > 0) {
            hitT = (corner.x - rayOrig.x) / rayDir.x;
        } else {
            hitT = (corner.x + length - rayOrig.x) / rayDir.x;
        }

        float yPos = rayOrig.y + rayDir.y * hitT;
        if (corner.y <= yPos && corner.y + length >= yPos) {
            float zPos = rayOrig.z + rayDir.z * hitT;
            if (corner.z <= zPos && corner.z + length >= zPos) {
                return true;
            }
        }
    }

    if (rayDir.y != 0) {
        if (rayDir.y > 0) {
            hitT = (corner.y - rayOrig.y) / rayDir.y;
        } else {
            hitT = (corner.y + length - rayOrig.y) / rayDir.y;
        }

        float xPos = rayOrig.x + rayDir.x * hitT;
        if (corner.x <= xPos && corner.x + length >= xPos) {
            float zPos = rayOrig.z + rayDir.z * hitT;
            if (corner.z <= zPos && corner.z + length >= zPos) {
                return true;
            }
        }
    }

    if (rayDir.z != 0) {
        if (rayDir.z > 0) {
            hitT = (corner.z - rayOrig.z) / rayDir.z;
        } else {
            hitT = (corner.z + length - rayOrig.z) / rayDir.z;
        }

        float yPos = rayOrig.y + rayDir.y * hitT;
        if (corner.y <= yPos && corner.y + length >= yPos) {
            float xPos = rayOrig.x + rayDir.x * hitT;
            if (corner.x <= xPos && corner.x + length >= xPos) {
                return true;
            }
        }
    }

    return false;
}

bool Voxel::testIntersection(HitData &data) {
    // see if intersects with voxel
    // if not, false
    // if it does, see if there are subvoxels
    // if there are no subvoxels, then it is a terminal hit
    // if there are subvoxels, see if it interacts with them
    // return data for closest subvoxel

    glm::vec3 rayOrig = data.getRayOrig();
    glm::vec3 rayDir = data.getRayDir();

    HitData retData(rayOrig, rayDir);
    retData.setObjDistSq(INFINITY);

    float hitT = 0.0f;

    bool hit = didHitVoxel(rayOrig, rayDir, hitT);

    if (hit) {
        glm::vec3 hPos = rayOrig + rayDir * hitT;
        retData.setHitPos(hPos);

        float dist = std::pow(rayOrig.x - hPos.x, 2) +
                     std::pow(rayOrig.y - hPos.y, 2) +
                     std::pow(rayOrig.z - hPos.z, 2);
        retData.setObjDistSq(dist);
    }

    data = retData;

    return hit;
}

bool Voxel::testSubVoxels(HitData &data) {

    glm::vec3 rayOrig = data.getRayOrig();
    glm::vec3 rayDir = data.getRayDir();

    HitData retData(rayOrig, rayDir);
    retData.setObjDistSq(INFINITY);

    // if it is the maximum depth, go through the contained triangles
    if (depth == maxDepth) {

        bool triFound = false;

        for (auto t : containedTris) {
            HitData tempData(rayOrig, rayDir);
            if (t->testIntersection(tempData)) {
                triFound = true;
                if (tempData.getObjDistSq() < retData.getObjDistSq()) {
                    retData = tempData;
                }
            }
        }

        if (triFound) {
            data = retData;
            data.setHitObj((void *)this);
        }
        return triFound;

    } else {

        // otherwise, go through the subvoxels
        bool subHit = false;

        std::vector<std::pair<std::shared_ptr<Voxel>, float>> closestSubVoxels;

        // find all of the subvoxels that are hit
        for (auto v : subVoxels) {
            HitData tempData(rayOrig, rayDir);
            if (v->testIntersection(tempData)) {
                subHit = true;
                closestSubVoxels.push_back(
                    std::make_pair(v, tempData.getObjDistSq()));
            }
        }

        if (subHit) {
            subHit = false;

            // sort voxels by distance from ray origin
            std::sort(closestSubVoxels.begin(), closestSubVoxels.end(),
                      [](auto &a, auto &b) { return a.second < b.second; });

            // test subvoxels of subvoxels in order of distance from origin
            for (auto p : closestSubVoxels) {
                auto v = p.first;
                HitData tempData(rayOrig, rayDir);
                if (v->testSubVoxels(tempData)) {
                    if (tempData.getObjDistSq() < retData.getObjDistSq()) {
                        retData = tempData;
                        subHit = true;
                        break;
                    }
                }
            }
        }

        // if a triangle within a subvoxel has been found
        if (subHit) {
            data = retData;
            data.setHitObj((void *)this);
        }

        return subHit;
    }

    return false;
}