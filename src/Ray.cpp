#include "Ray.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    orig = origin;
    dir = direction;
}

Ray::~Ray() {}

glm::vec3 Ray::traceRay(Object *o) {

    if (o->testIntersection(orig, dir)) {
        return glm::vec3(1, 1, 1);
    }

    return glm::vec3(0, 0, 0);
}