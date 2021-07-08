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

// calculate ray hit color using phong
glm::vec3 Ray::calcColor(Scene &s, HitData &data, int depth) {

    Material *objMat = data.getHitMat();

    glm::vec3 retCol =
        s.getAmbientStrength() * s.getAmbientColor() * objMat->getDiffuse();

    glm::vec3 hitPos = data.getHitPos();
    glm::vec3 hitNorm = data.getHitNormal();

    glm::vec3 viewDir = glm::normalize(orig - hitPos);

    for (auto l : s.getLights()) {

        glm::vec3 lPos = l->getPos();

        glm::vec3 lightDir = glm::normalize(lPos - hitPos);

        // we add a little bit of the normal to the hit position so we don't get
        // self-intersections
        Ray shadowRay(hitPos + (hitNorm * 0.0001f), lightDir);
        float interDist = 0.0f;
        bool inter = shadowRay.isThereIntersection(s, &interDist);

        float lDist = std::pow(hitPos.x - lPos.x, 2) +
                      std::pow(hitPos.y - lPos.y, 2) +
                      std::pow(hitPos.z - lPos.z, 2);

        // if there is nothing between the hit and the light, apply lighting
        if (!inter || interDist > lDist) {
            glm::vec3 diffSpec = glm::vec3(0, 0, 0);

            glm::vec3 diffuse = objMat->getDiffuse() * l->getCol();
            diffuse = diffuse * std::max(glm::dot(lightDir, hitNorm), 0.0f);
            diffuse *= l->getIntensity();

            diffSpec += diffuse;

            glm::vec3 half = glm::normalize(viewDir + lightDir);
            glm::vec3 specular = objMat->getSpecular() * l->getCol();
            specular =
                specular * std::pow(std::max(glm::dot(half, hitNorm), 0.0f),
                                    objMat->getPhongExponent());

            diffSpec += specular;

            retCol += diffSpec;
        }
    }

    constexpr int MAX_DEPTH = 3;

    // reflections
    if (depth < MAX_DEPTH && objMat->getUseReflection()) {
        glm::vec3 oppViewDir = -1.0f * viewDir;
        glm::vec3 reflectDir =
            oppViewDir - 2.0f * glm::dot(oppViewDir, hitNorm) * hitNorm;

        Ray reflectRay(hitPos + hitNorm * 0.0001f, reflectDir);
        retCol +=
            objMat->getReflectionCoef() * reflectRay.traceRay(s, depth + 1);
    }

    retCol = glm::clamp(retCol, glm::vec3(0.0f), glm::vec3(1.0f));

    return retCol;
}

glm::vec3 Ray::traceRay(Scene &s, int depth) {

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
        return calcColor(s, closestData, depth);
    }

    return s.getAmbientColor() * s.getAmbientStrength();
}