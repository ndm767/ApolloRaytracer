#pragma once

#include <GL/glew.h>

class DisplayShader {
  public:
    DisplayShader();
    ~DisplayShader();

    void use();

  protected:
    GLuint program;
};