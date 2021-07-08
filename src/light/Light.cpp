#include "Light.hpp"

Light::Light(glm::vec3 position, float intensity, glm::vec3 color) {
    pos = position;
    col = color;
    inten = intensity;
}

Light::~Light() {}