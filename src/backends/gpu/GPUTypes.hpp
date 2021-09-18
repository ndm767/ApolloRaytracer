#pragma once

#include <glm/glm.hpp>

// simple types for transferring information from cpu to gpu
struct GPURay {
    glm::vec4 orig;
    glm::vec4 dir;
};

struct GPUSphere {
    glm::vec4 center;
    glm::vec4 radius;
};

struct GPUTriangle {
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;
};

struct GPUAABB {
    glm::vec4 minPos;
    glm::vec4 maxPos;
    glm::ivec4 triPos;
};