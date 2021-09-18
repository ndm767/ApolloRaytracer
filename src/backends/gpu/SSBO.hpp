#pragma once

#include <GL/glew.h>

#include <vector>

#include "GPUTypes.hpp"
#include <glm/glm.hpp>

template <typename T>
class SSBO {
  public:
    SSBO(std::vector<T> data, int _binding);
    ~SSBO();

    std::vector<T> read();
    void bind();

    void modify(std::vector<T> data);

  protected:
    int binding;
    int size;
    GLuint buffer;
};

template class SSBO<float>;
template class SSBO<glm::vec4>;
template class SSBO<GPURay>;
template class SSBO<GPUSphere>;
template class SSBO<GPUTriangle>;
template class SSBO<GPUAABB>;