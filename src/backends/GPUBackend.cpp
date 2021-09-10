#include "GPUBackend.hpp"

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

    std::vector<float> testDat = {1.0f, 0.0f, 0.0f};
    shader->createSSBO(testDat, 0);

    std::vector<float> outDat;
    for (int i = 0; i < h * w * 3; i++) {
        outDat.push_back(0.0f);
    }
    shader->createSSBO(outDat, 1);

    shader->dispatch(w, h);

    outDat = shader->readSSBO(1, w * h * 3);
    std::cout << outDat.at(0) << " " << outDat.at(1) << " " << outDat.at(2)
              << std::endl;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            d->drawPixel(x, y,
                         glm::vec3(outDat.at((400 * y + x) * 3),
                                   outDat.at((400 * y + x) * 3 + 1),
                                   outDat.at((400 * y + x) * 3 + 2)));
        }
    }
}