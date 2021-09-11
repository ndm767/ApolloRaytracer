#pragma once

#include <GL/glew.h>

#include <vector>

template <typename T>
class SSBO {
  public:
    SSBO(std::vector<T> data, int _binding);
    ~SSBO();

    std::vector<T> read();

  protected:
    int binding;
    int size;
    GLuint buffer;
};

template class SSBO<float>;