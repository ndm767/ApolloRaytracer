#include "CPUBackend.hpp"

#include "../Ray.hpp"

#include <future>

CPUBackend::CPUBackend(int width, int height, int numSamples, int resolution) {
    w = width;
    h = height;
    numSamp = numSamples;
    res = resolution;
}

CPUBackend::~CPUBackend() {}

void CPUBackend::drawColumn(Display *d, Camera *c, Scene *s, int x) {
    for (int y = 0; y < h; y += res) {
        glm::vec3 result = glm::vec3(0);
        float sampDist = float(res) / numSamp;
        for (int n = 0; n < numSamp; n++) {
            Ray r = c->getRayAtPixel(x + sampDist * n, y + sampDist * n);
            result += r.traceRay(*s);
        }

        for (int i = 0; i < res; i++) {
            for (int j = 0; j < res; j++) {
                d->drawPixel(x + i, y + j, result / float(numSamp));
            }
        }
    }
}

void CPUBackend::drawPixels(Display *d, Scene *s) {
    Camera *currCam = s->getActiveCamera().get();
    std::vector<std::future<void>> threads;

    // makes each column of pixels its own thread
    for (int x = 0; x < w; x++) {
        threads.push_back(
            std::async(&CPUBackend::drawColumn, this, d, currCam, s, x));
    }

    // wait for threads to finish
    for (auto &t : threads) {
        t.wait();
    }
}

void CPUBackend::render(Scene *s, Display *d) { drawPixels(d, s); }