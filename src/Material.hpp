#pragma once

#include <glm/glm.hpp>

// Material/Color data for an object
class Material {
  public:
    Material();
    Material(glm::vec3 diff, glm::vec3 spec, float exp);
    ~Material();

    // getters
    glm::vec3 getDiffuse() { return diffuse; }
    glm::vec3 getSpecular() { return specular; }

    float getPhongExponent() { return phongExponent; }

    bool getUseReflection() { return useReflection; }
    float getReflectionCoef() { return reflectionCoef; }

    bool getUseRefraction() { return useRefraction; }
    float getRefractionCoef() { return refractionCoef; }
    float getIndexOfRefraction() { return indexOfRefraction; }

    // setters
    void setDiffuse(glm::vec3 color) { diffuse = color; }
    void setSpecular(glm::vec3 color) { specular = color; }

    void setPhongExponent(float exponent) { phongExponent = exponent; }

    void setUseReflection(bool val) { useReflection = val; }
    void setReflectionCoef(float coef) { reflectionCoef = coef; }

    void setUseRefraction(bool val) { useRefraction = val; }
    void setRefractionCoef(float coef) { refractionCoef = coef; }
    void setIndexOfRefraction(float ior) { indexOfRefraction = ior; }

  protected:
    glm::vec3 diffuse;
    glm::vec3 specular;

    float phongExponent;

    bool useReflection;
    float reflectionCoef;

    bool useRefraction;
    float refractionCoef;
    float indexOfRefraction;
};