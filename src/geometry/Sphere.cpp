#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 center, float radius) {
    r = radius;
    pos = center;
}

Sphere::~Sphere() {}

// see if ray intersects with sphere
bool Sphere::testIntersection(HitData &data) {

    glm::vec3 rayDir = data.getRayDir();
    glm::vec3 rayOrig = data.getRayOrig();

    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(rayDir, rayOrig - pos);
    float c = glm::dot(rayOrig - pos, rayOrig - pos) - r * r;

    float underSquare = b * b - 4 * a * c;
    // see if we would be taking the square root of a negative number
    if (underSquare < 0 || a == 0) {
        return false;
    } else {
        float tPos = (-1.0f * b + sqrt(underSquare)) / (2.0f * a);
        float tNeg = (-1.0f * b - sqrt(underSquare)) / (2.0f * a);

        // if one of the intersections is behind the camera
        if (tNeg < 0) {
            glm::vec3 hitPos = rayOrig + tPos * rayDir;
            glm::vec3 hitNorm = hitPos - pos;

            data.setHitPos(hitPos);
            data.setHitNormal(hitNorm);

            float distSq = (hitPos.x - rayOrig.x) * (hitPos.x - rayOrig.x) +
                           (hitPos.y - rayOrig.y) * (hitPos.y - rayOrig.y) +
                           (hitPos.z - rayOrig.z) * (hitPos.z - rayOrig.z);
            data.setObjDistSq(distSq);

            return true;
        } else {
            // figure out which intersection is closer and go with that one
            glm::vec3 hitPosPos = rayOrig + tPos * rayDir;
            glm::vec3 hitPosNeg = rayOrig + tNeg * rayDir;

            float distSqPos =
                (hitPosPos.x - rayOrig.x) * (hitPosPos.x - rayOrig.x) +
                (hitPosPos.y - rayOrig.y) * (hitPosPos.y - rayOrig.y) +
                (hitPosPos.z - rayOrig.z) * (hitPosPos.z - rayOrig.z);

            float distSqNeg =
                (hitPosNeg.x - rayOrig.x) * (hitPosNeg.x - rayOrig.x) +
                (hitPosNeg.y - rayOrig.y) * (hitPosNeg.y - rayOrig.y) +
                (hitPosNeg.z - rayOrig.z) * (hitPosNeg.z - rayOrig.z);

            if (distSqPos > distSqNeg) {
                data.setHitPos(hitPosNeg);
                data.setHitNormal(hitPosNeg - pos);
                data.setObjDistSq(distSqNeg);
            } else {
                data.setHitPos(hitPosPos);
                data.setHitNormal(hitPosPos - pos);
                data.setObjDistSq(distSqPos);
            }
            return true;
        }
    }
}