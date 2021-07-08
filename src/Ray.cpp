#include "Ray.hpp"
#include "HitData.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    orig = origin;
    dir = glm::normalize(direction);
}

Ray::~Ray() {}

glm::vec3 Ray::traceRay(Scene &s) {

    HitData lastData(orig, dir);
    lastData.setObjDistSq(INFINITY);

    bool foundObj = false;

    for (auto o : s.getObjects()) {
        HitData tempData(orig, dir);
        if (o->testIntersection(tempData)) {
            foundObj = true;
            if (tempData.getObjDistSq() < lastData.getObjDistSq()) {
                lastData = tempData;
            }
        }
    }

    if (foundObj) {
        return glm::vec3(1, 1, 1);
    }

    return glm::vec3(0, 0, 0);
}