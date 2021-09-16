#pragma once

#include <glm/glm.hpp>

// simple types for transferring information from cpu to gpu
struct GPURay {
    glm::vec4 orig;
    glm::vec4 dir;
};

struct GPUSphere {
    glm::vec4 center;
    float radius;
};

struct GPUTriangle {
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;
};