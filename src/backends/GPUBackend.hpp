#pragma once

#include "../Material.hpp"
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

    glm::vec3 getHitColor(GPURetData hitData, Scene *s);
    void drawColumn(int x, Scene *s, Display *d,
                    std::vector<GPURetData> outDat);

  protected:
    ComputeShader *shader;
    bool buffersMade;
    std::vector<Material> mats;
    // buffers
    SSBO<GPURetData> *outBuf;
    SSBO<GPURay> *rayBuf;
    SSBO<GPUSphere> *sphereBuf;
    SSBO<GPUTriangle> *triBuf;
    SSBO<GPUAABB> *boxBuf;
};