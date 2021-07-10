#pragma once

#include <glm/glm.hpp>

#include "Scene.hpp"

class Ray {
  public:
    Ray(glm::vec3 origin, glm::vec3 direction, int _maxDepth,
        void *_skipObj = nullptr);
    ~Ray();

    glm::vec3 traceRay(Scene &s, int depth = 0);

    glm::vec3 getOrig() { return orig; }
    glm::vec3 getDir() { return dir; }

    // this method does the same thing as traceRay() except it only returns
    // whether or not there was an intersection and the distance to the
    // intersection
    bool isThereIntersection(Scene &s, float *distSq);

  protected:
    // calculate the color returned by the ray
    glm::vec3 calcColor(Scene &s, HitData &data, int depth);

    glm::vec3 orig;
    glm::vec3 dir;

    int maxDepth;

    void *skipObj;
};