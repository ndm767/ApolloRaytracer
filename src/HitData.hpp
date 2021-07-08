#pragma once

#include <glm/glm.hpp>

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
    glm::vec3 getHitColor() { return hitColor; }

    float getObjDistSq() { return objDistSq; }

    void setHitPos(glm::vec3 pos) { hitPos = pos; }
    void setHitNormal(glm::vec3 norm) { hitNormal = norm; }
    void setHitColor(glm::vec3 color) { hitColor = color; }
    void setObjDistSq(float distSq) { objDistSq = distSq; }

  protected:
    glm::vec3 rayOrig;
    glm::vec3 rayDir;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;
    glm::vec3 hitColor;
    float objDistSq;
};