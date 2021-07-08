#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 center, float radius) {
    r = radius;
    pos = center;
}

Sphere::~Sphere() {}

// see if ray intersects with sphere
bool Sphere::testIntersection(glm::vec3 rayOrig, glm::vec3 rayDir) {
    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(rayDir, rayOrig - pos);
    float c = glm::dot(rayOrig - pos, rayOrig - pos) - r * r;

    if (b * b - 4 * a * c < 0) {
        return false;
    } else {
        return true;
    }
}