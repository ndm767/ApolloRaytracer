#include "Triangle.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Triangle::Triangle(glm::vec3 points[3], Material mat, glm::vec3 triNorm) {
    p[0] = points[0];
    p[1] = points[1];
    p[2] = points[2];

    // get the average of the vertices to find the position
    pos = (p[0] + p[1] + p[2]) / 3.0f;

    if (triNorm == glm::vec3(0.0f, 0.0f, 0.0f)) {
        norm = glm::cross(p[1] - p[0], p[2] - p[0]);
        norm = glm::normalize(norm);
    } else {
        norm = triNorm;
    }

    objMat = mat;
}

Triangle::~Triangle() {}

// see if ray intersects with triangle
bool Triangle::testIntersection(HitData &data) {
    // algorithm taken from Section 4.4.2 of Fundamentals of Computer Graphics
    // 4ed
    glm::vec3 rayDir = data.getRayDir();
    glm::vec3 rayOrig = data.getRayOrig();

    // clang-format off
    float aMat[] = {
        p[0].x - p[1].x, p[0].x - p[2].x, rayDir.x,
        p[0].y - p[1].y, p[0].y - p[2].y, rayDir.y,
        p[0].z - p[1].z, p[0].z - p[2].z, rayDir.z
    };
    // clang-format on
    float detA = glm::determinant(glm::make_mat3(aMat));

    // clang-format off
    float tMat[] = {
        p[0].x - p[1].x, p[0].x - p[2].x, p[0].x - rayOrig.x,
        p[0].y - p[1].y, p[0].y - p[2].y, p[0].y - rayOrig.y,
        p[0].z - p[1].z, p[0].z - p[2].z, p[0].z - rayOrig.z
    };
    // clang-format on

    float t = glm::determinant(glm::make_mat3(tMat)) / detA;
    if (t < 0) {
        return false;
    }

    // clang-format off
    float gMat[] = {
        p[0].x - p[1].x, p[0].x - rayOrig.x, rayDir.x,
        p[0].y - p[1].y, p[0].y - rayOrig.y, rayDir.y,
        p[0].z - p[1].z, p[0].z - rayOrig.z, rayDir.z
    };
    // clang-format on

    float gamma = glm::determinant(glm::make_mat3(gMat)) / detA;
    if (gamma < 0 || gamma > 1) {
        return false;
    }

    // clang-format off
    float bMat[] = {
        p[0].x - rayOrig.x, p[0].x - p[2].x, rayDir.x,
        p[0].y - rayOrig.y, p[0].y - p[2].y, rayDir.y,
        p[0].z - rayOrig.z, p[0].z - p[2].z, rayDir.z
    };
    // clang-format on

    float beta = glm::determinant(glm::make_mat3(bMat)) / detA;
    if (beta < 0 || beta > 1 - gamma) {
        return false;
    }

    glm::vec3 hitPos = rayOrig + rayDir * t;
    float distSq = std::pow(rayOrig.x - hitPos.x, 2) +
                   std::pow(rayOrig.y - hitPos.y, 2) +
                   std::pow(rayOrig.z - hitPos.z, 2);

    data.setHitMat(&objMat);
    data.setHitNormal(norm);
    data.setHitPos(hitPos);
    data.setObjDistSq(distSq);
    return true;
}
