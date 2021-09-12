#include "GPUBackend.hpp"

#include "../Ray.hpp"
#include "gpu/GPUTypes.hpp"
#include "gpu/HitShader.hpp"

#include <GL/glew.h>

#include <iostream>

GPUBackend::GPUBackend(int width, int height, int numSamples, int resolution) {
    w = width;
    h = height;
    numSamp = numSamples;
    res = resolution;

    shader = new ComputeShader(hitShaderSrc);
}

GPUBackend::~GPUBackend() { delete shader; }

void GPUBackend::render(Scene *s, Display *d) {

    shader->useProgram();

    Camera *cam = s->getActiveCamera().get();

    std::vector<GPURay> rayDat;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Ray r = cam->getRayAtPixel((float)x, (float)y);
            GPURay gRay;
            gRay.orig = glm::vec4(r.getOrig(), 1.0);
            gRay.dir = glm::vec4(r.getDir(), 1.0);
            rayDat.push_back(gRay);
        }
    }

    SSBO<GPURay> rayBuf(rayDat, 0);

    std::vector<float> outDat;
    for (int i = 0; i < h * w * 3; i++) {
        outDat.push_back(0.0f);
    }
    SSBO<float> outBuf(outDat, 1);

    shader->dispatch(w, h);

    outDat = outBuf.read();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            d->drawPixel(x, y,
                         glm::vec3(outDat.at((400 * y + x) * 3),
                                   outDat.at((400 * y + x) * 3 + 1),
                                   outDat.at((400 * y + x) * 3 + 2)));
        }
    }
}