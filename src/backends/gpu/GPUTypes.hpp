#pragma once

#include <glm/glm.hpp>

// simple types for transferring information from cpu to gpu
struct GPURay {
    glm::vec4 orig;
    glm::vec4 dir;
};