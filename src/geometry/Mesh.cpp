#include "Mesh.hpp"

#include <algorithm>
#include <iostream>

Mesh::Mesh(MeshInput vals, glm::vec3 meshPos, float scale, Material mat) {
    objMat = mat;

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

    for (auto t : tris) {
        HitData tempData(data.getRayOrig(), data.getRayDir());
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
}