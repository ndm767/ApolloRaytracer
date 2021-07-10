#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class HitData {
  public:
    HitData(glm::vec3 orig, glm::vec3 dir) {
        rayOrig = orig;
        rayDir = dir;
    }
    ~HitData() {}

    glm::vec3 getRayOrig() { return rayOrig; }
    glm::vec3 getRayDir() { return rayDir; }

    glm::vec3 getHitPos() { return hitPos; }
    glm::vec3 getHitNormal() { return hitNormal; }

    Material *getHitMat() { return hitMat; }

    float getObjDistSq() { return objDistSq; }

    void *getHitObj() { return hitObj; }

    void setHitPos(glm::vec3 pos) { hitPos = pos; }
    void setHitNormal(glm::vec3 norm) { hitNormal = norm; }
    void setObjDistSq(float distSq) { objDistSq = distSq; }
    void setHitMat(Material *mat) { hitMat = mat; }

    void setHitObj(void *obj) { hitObj = obj; }

  protected:
    glm::vec3 rayOrig;
    glm::vec3 rayDir;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;
    float objDistSq;
    Material *hitMat;
    void *hitObj;
};