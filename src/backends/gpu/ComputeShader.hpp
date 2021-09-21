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

    void setUniform1i(std::string name, int value);

  protected:
    GLuint program;
};