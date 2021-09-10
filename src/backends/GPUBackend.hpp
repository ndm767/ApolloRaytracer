#pragma once

#include "backend.hpp"
#include "gpu/ComputeShader.hpp"

class GPUBackend : public Backend {
  public:
    GPUBackend(int width, int height, int numSamples, int resolution);
    ~GPUBackend();

    void render(Scene *s, Display *d) override;

  protected:
    ComputeShader *shader;
};