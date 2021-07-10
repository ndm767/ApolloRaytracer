#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <iostream>

Mesh::Mesh(MeshInput vals, glm::vec3 meshPos, float scale, Material mat) {
    objMat = mat;
    pos = glm::vec3(0);

    for (unsigned i = 0; i < vals.indices.size(); i += 3) {
        glm::vec3 points[3] = {vals.vertices.at(vals.indices.at(i)),
                               vals.vertices.at(vals.indices.at(i + 1)),
                               vals.vertices.at(vals.indices.at(i + 2))};
        for (int j = 0; j < 3; j++) {
            points[j] = points[j] * scale + meshPos;
        }
        glm::vec3 faceNorm = vals.normals.at(vals.indices.at(i));
        faceNorm += vals.normals.at(vals.indices.at(i + 1));
        faceNorm += vals.normals.at(vals.indices.at(i + 2));
        faceNorm /= 3.0f;

        tris.push_back(std::make_shared<Triangle>(points, objMat, faceNorm));
    }

    // make bounding box and get position
    std::vector<float> xVals;
    std::vector<float> yVals;
    std::vector<float> zVals;

    for (auto v : vals.vertices) {
        v = v * scale + meshPos;
        xVals.push_back(v.x);
        yVals.push_back(v.y);
        zVals.push_back(v.z);
        pos += v;
    }

    pos /= vals.vertices.size();

    glm::vec3 maxCoord;
    maxCoord.x = *std::max_element(xVals.begin(), xVals.end());
    maxCoord.y = *std::max_element(yVals.begin(), yVals.end());
    maxCoord.z = *std::max_element(zVals.begin(), zVals.end());

    glm::vec3 minCoord;
    minCoord.x = *std::min_element(xVals.begin(), xVals.end());
    minCoord.y = *std::min_element(yVals.begin(), yVals.end());
    minCoord.z = *std::min_element(zVals.begin(), zVals.end());

    std::cout << "Min: " << minCoord.x << " " << minCoord.y << " " << minCoord.z
              << std::endl;
    std::cout << "Max: " << maxCoord.x << " " << maxCoord.y << " " << maxCoord.z
              << std::endl;

    bb = std::make_shared<BoundingBox>(minCoord, maxCoord);
}

Mesh::~Mesh() {}

bool Mesh::testIntersection(HitData &data) {
    HitData retData(data.getRayOrig(), data.getRayDir());
    retData.setObjDistSq(INFINITY);

    if (!bb->testIntersection(retData)) {
        return false;
    }

    bool triFound = false;
    // sort triangles by how close they are to a projected hit position
    glm::vec3 dirToMesh = pos - data.getRayOrig();
    glm::vec3 step = dirToMesh / glm::normalize(dirToMesh);
    float coefToMesh = std::max(step.x, std::max(step.y, step.z));
    glm::vec3 projectedHit = data.getRayOrig() + data.getRayDir() * coefToMesh;

    struct TriDat {
        glm::vec3 p[3];
        float closestDistSq;
        std::shared_ptr<Triangle> ptr;
    };

    auto getCDS = [](glm::vec3 &hit, TriDat &dat) {
        float dist1 = std::pow(hit.x - dat.p[0].x, 2) +
                      std::pow(hit.y - dat.p[0].y, 2) +
                      std::pow(hit.z - dat.p[0].z, 2);

        float dist2 = std::pow(hit.x - dat.p[1].x, 2) +
                      std::pow(hit.y - dat.p[1].y, 2) +
                      std::pow(hit.z - dat.p[1].z, 2);

        float dist3 = std::pow(hit.x - dat.p[2].x, 2) +
                      std::pow(hit.y - dat.p[2].y, 2) +
                      std::pow(hit.z - dat.p[2].z, 2);
        return std::max(std::max(dist1, dist2), dist3);
    };

    std::vector<TriDat> trisCopy;
    for (auto t : tris) {
        TriDat temp;
        glm::vec3 *p = t.get()->getPoints();
        temp.p[0] = *p;
        temp.p[1] = *(p + 1);
        temp.p[2] = *(p + 2);
        temp.ptr = t;
        temp.closestDistSq = getCDS(projectedHit, temp);
        trisCopy.push_back(temp);
    }

    std::sort(trisCopy.begin(), trisCopy.end(), [](TriDat &a, TriDat &b) {
        return a.closestDistSq < b.closestDistSq;
    });

    for (auto &t : trisCopy) {
        HitData tempData(data.getRayOrig(), data.getRayDir());
        if (t.ptr->testIntersection(tempData)) {
            triFound = true;
            if (tempData.getObjDistSq() < retData.getObjDistSq()) {
                retData = tempData;
                break;
            }
        }
    }

    if (triFound) {
        data = retData;
    }
    return triFound;
}