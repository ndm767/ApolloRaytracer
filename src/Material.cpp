#include "Material.hpp"

Material::Material() {}

Material::Material(glm::vec3 diff, glm::vec3 spec, float exp) {
    diffuse = diff;
    specular = spec;
    phongExponent = exp;

    useReflection = false;
}

Material::~Material() {}