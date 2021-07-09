#include "BoundingBox.hpp"

BoundingBox::BoundingBox(glm::vec3 _minPos, glm::vec3 _maxPos) {
    minPos = _minPos;
    maxPos = _maxPos;
}

BoundingBox::~BoundingBox() {}

bool BoundingBox::testIntersection(HitData &data) {
    glm::vec3 rayOrig = data.getRayOrig();
    glm::vec3 rayDir = data.getRayDir();

    if (rayDir.x != 0) {
        float t = 0.0f;
        if (rayDir.x > 0) {
            t = (minPos.x - rayOrig.x) / rayDir.x;
        } else {
            t = (maxPos.x - rayOrig.x) / rayDir.x;
        }

        float yPos = rayOrig.y + rayDir.y * t;
        if (minPos.y <= yPos && maxPos.y >= yPos) {
            float zPos = rayOrig.z + rayDir.z * t;
            if (minPos.z <= zPos && maxPos.z >= zPos) {
                return true;
            }
        }
    }

    if (rayDir.y != 0) {
        float t = 0.0f;
        if (rayDir.y > 0) {
            t = (minPos.y - rayOrig.y) / rayDir.y;
        } else {
            t = (maxPos.y - rayOrig.y) / rayDir.y;
        }

        float xPos = rayOrig.x + rayDir.x * t;
        if (minPos.x <= xPos && maxPos.x >= xPos) {
            float zPos = rayOrig.z + rayDir.z * t;
            if (minPos.z <= zPos && maxPos.z >= zPos) {
                return true;
            }
        }
    }

    if (rayDir.z != 0) {
        float t = 0.0f;
        if (rayDir.z > 0) {
            t = (minPos.z - rayOrig.z) / rayDir.z;
        } else {
            t = (maxPos.z - rayOrig.z) / rayDir.z;
        }

        float yPos = rayOrig.y + rayDir.y * t;
        if (minPos.y <= yPos && maxPos.y >= yPos) {
            float xPos = rayOrig.x + rayDir.x * t;
            if (minPos.x <= xPos && maxPos.x >= xPos) {
                return true;
            }
        }
    }

    return false;
}