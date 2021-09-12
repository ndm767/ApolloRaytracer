#pragma once

#include <GL/glew.h>

class QuadRenderer {
  public:
    QuadRenderer();
    ~QuadRenderer();

    void render();

  protected:
    GLuint VAO, VBO, texVBO;
};