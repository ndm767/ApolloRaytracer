#include "Ray.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    orig = origin;
    dir = glm::normalize(direction);
}

Ray::~Ray() {}

glm::vec3 Ray::traceRay(Scene &s) {

    for (auto o : s.getObjects()) {
        if (o->testIntersection(orig, dir)) {
            return glm::vec3(1, 1, 1);
        }
    }

    return glm::vec3(0, 0, 0);
}