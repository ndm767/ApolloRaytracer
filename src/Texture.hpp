#pragma once

#include <glm/glm.hpp>
#include <string>

class Texture {
  public:
    Texture(std::string path);
    ~Texture();

    glm::vec3 getColorAtPos(float u, float v);

  protected:
    int width, height;
    int numChannels;
    unsigned char *texData;

    bool texFailed;
};