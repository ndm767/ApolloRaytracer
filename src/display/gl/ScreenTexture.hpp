#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <vector>

class ScreenTexture {
  public:
    ScreenTexture(std::vector<std::vector<glm::vec3>> &pixels);
    ~ScreenTexture();

    void set(std::vector<std::vector<glm::vec3>> &pixels);
    void use();

  protected:
    int w, h;
    GLuint texture;
    unsigned char *data;
};