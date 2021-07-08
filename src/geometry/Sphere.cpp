#include "Sphere.hpp"

#include <iostream>

Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 color) {
    r = radius;
    pos = center;
    col = color;
}

Sphere::~Sphere() {}

// see if ray intersects with sphere
bool Sphere::testIntersection(HitData &data) {
    // this is done by creating a quadratic equation to relate the line equation
    // and the sphere equation and then solving for x with the quadratic
    // formula, if x exists and is greater than 0, then the object intersects
    // the ray in the forward direction
    glm::vec3 rayDir = data.getRayDir();
    glm::vec3 rayOrig = data.getRayOrig();

    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(rayDir, rayOrig - pos);
    float c = glm::dot(rayOrig - pos, rayOrig - pos) - r * r;

    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float quadraticTop = -b - std::sqrt(discriminant);
        float coef = 0.0f;
        if (quadraticTop > 0.0f) {
            coef = quadraticTop;
        } else {
            quadraticTop = -b + std::sqrt(discriminant);
            coef = quadraticTop;
        }

        // coef is now the solution to the quadratic equation
        coef /= (2 * a);

        float epsilon = std::pow(2.0f, -52.0f);
        if (coef > epsilon) {
            glm::vec3 hitPos = rayOrig + rayDir * coef;
            glm::vec3 hitNorm = glm::normalize(hitPos - pos);

            float distSq = std::pow(rayOrig.x - hitPos.x, 2) +
                           std::pow(rayOrig.y - hitPos.y, 2) +
                           std::pow(rayOrig.z - hitPos.z, 2);

            data.setHitColor(col);
            data.setHitNormal(hitNorm);
            data.setHitPos(hitPos);
            data.setObjDistSq(distSq);
            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}