#include "Ray.hpp"
#include "HitData.hpp"

#include <iostream>

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    orig = origin;
    dir = glm::normalize(direction);
}

Ray::~Ray() {}

bool Ray::isThereIntersection(Scene &s, float *distSq) {
    HitData closestData(orig, dir);
    closestData.setObjDistSq(INFINITY);

    bool foundObj = false;

    for (auto o : s.getObjects()) {
        HitData tempData(orig, dir);
        if (o->testIntersection(tempData)) {
            foundObj = true;
            if (tempData.getObjDistSq() < closestData.getObjDistSq()) {
                closestData = tempData;
            }
        }
    }

    *distSq = closestData.getObjDistSq();
    return foundObj;
}

// calculate ray hit color
glm::vec3 Ray::calcColor(Scene &s, HitData &data) {
    float ambientStrength = 0.25f;
    glm::vec3 retCol = ambientStrength * data.getHitColor();

    glm::vec3 hitPos = data.getHitPos();
    glm::vec3 hitNorm = data.getHitNormal();

    for (auto l : s.getLights()) {

        glm::vec3 lPos = l->getPos();

        glm::vec3 lightDir = lPos - hitPos;

        // we add a little bit of the normal to the hit position so we don't get
        // self-intersections
        Ray shadowRay(hitPos + (hitNorm * 0.0001f), lightDir);
        float interDist = 0.0f;
        bool inter = shadowRay.isThereIntersection(s, &interDist);

        float lDist = std::pow(hitPos.x - lPos.x, 2) +
                      std::pow(hitPos.y - lPos.y, 2) +
                      std::pow(hitPos.z - lPos.z, 2);

        if (!inter || interDist > lDist) {
            glm::vec3 diffuse = data.getHitColor() * l->getCol();
            diffuse = diffuse * std::max(glm::dot(lightDir, hitNorm), 0.0f);
            diffuse *= l->getIntensity();

            retCol += diffuse;
        }
    }

    retCol = glm::clamp(retCol, glm::vec3(0.0f), glm::vec3(1.0f));

    return retCol;
}

glm::vec3 Ray::traceRay(Scene &s) {

    HitData closestData(orig, dir);
    closestData.setObjDistSq(INFINITY);

    bool foundObj = false;

    for (auto o : s.getObjects()) {
        HitData tempData(orig, dir);
        if (o->testIntersection(tempData)) {
            foundObj = true;
            if (tempData.getObjDistSq() < closestData.getObjDistSq()) {
                closestData = tempData;
            }
        }
    }

    if (foundObj) {
        return calcColor(s, closestData);
    }

    return glm::vec3(0, 0, 0);
}