#include "GPUBackend.hpp"

#include "gpu/GPUTypes.hpp"
#include "gpu/HitShader.hpp"

#include "../Ray.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Sphere.hpp"

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

    // ray input
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

    SSBO<GPURay> rayBuf(rayDat, 1);

    // sphere input
    std::vector<GPUSphere> spheres;
    std::vector<std::shared_ptr<Object>> objs = s->getObjects();
    for (auto &o : objs) {
        auto oPtr = o.get();
        Sphere *sphereCast = dynamic_cast<Sphere *>(oPtr);
        if (sphereCast != nullptr) {
            GPUSphere sp;
            sp.center = glm::vec4(sphereCast->getCenter(), 1.0f);
            sp.radius = sphereCast->getRadius();
            spheres.push_back(sp);
        }
    }

    SSBO<GPUSphere> sphereBuf(spheres, 2);

    // triangle input
    std::vector<GPUTriangle> tris;
    for (auto &o : objs) {
        auto oPtr = o.get();
        Mesh *meshCast = dynamic_cast<Mesh *>(oPtr);
        if (meshCast != nullptr) {
            auto meshTris = meshCast->getTris();
            for (auto &t : meshTris) {
                GPUTriangle currTri;
                auto triArray = t.get()->getPoints();
                currTri.p1 = glm::vec4(triArray.at(0), 1.0f);
                currTri.p2 = glm::vec4(triArray.at(1), 1.0f);
                currTri.p3 = glm::vec4(triArray.at(2), 1.0f);
                tris.push_back(currTri);
            }
        }
    }

    SSBO<GPUTriangle> triBuf(tris, 3);

    // output
    std::vector<float> outDat;
    for (int i = 0; i < h * w * 3; i++) {
        outDat.push_back(0.0f);
    }
    SSBO<float> outBuf(outDat, 0);

    shader->dispatch(w, h);

    outDat = outBuf.read();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            d->drawPixel(x, y,
                         glm::vec3(outDat.at((400 * x + y) * 3),
                                   outDat.at((400 * x + y) * 3 + 1),
                                   outDat.at((400 * x + y) * 3 + 2)));
        }
    }
}