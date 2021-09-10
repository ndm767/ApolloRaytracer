#pragma once

#include <GL/glew.h>

#include <string>
#include <utility>
#include <vector>

class ComputeShader {
  public:
    ComputeShader(std::string compSrc);
    ~ComputeShader();

    void createSSBO(std::vector<float> data, int binding);
    std::vector<float> readSSBO(int binding, int size);

    void useProgram();
    void dispatch(int width, int height);

  protected:
    GLuint program;
    std::vector<std::pair<int, GLuint>> ssbos;
};