#pragma once

#include "backend.hpp"
#include "gpu/ComputeShader.hpp"
#include "gpu/SSBO.hpp"

#include <variant>

class GPUBackend : public Backend {
  public:
    GPUBackend(int width, int height, int numSamples, int resolution);
    ~GPUBackend();

    void createBuffers(Scene *s);
    void render(Scene *s, Display *d) override;

  protected:
    ComputeShader *shader;
    bool buffersMade;
    // buffers
    SSBO<float> *outBuf;
    SSBO<GPURay> *rayBuf;
    SSBO<GPUSphere> *sphereBuf;
    SSBO<GPUTriangle> *triBuf;
    SSBO<GPUAABB> *boxBuf;
};