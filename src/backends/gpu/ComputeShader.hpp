#pragma once

#include <GL/glew.h>

#include <string>
#include <utility>
#include <vector>

class ComputeShader {
  public:
    ComputeShader(std::string compSrc);
    ~ComputeShader();

    void useProgram();
    void dispatch(int width, int height);

  protected:
    GLuint program;
};