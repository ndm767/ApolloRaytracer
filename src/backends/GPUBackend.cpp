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

    buffersMade = false;
}

GPUBackend::~GPUBackend() {
    delete triBuf;
    delete boxBuf;
    delete sphereBuf;
    delete rayBuf;
    delete outBuf;
    delete shader;
}

void GPUBackend::createBuffers(Scene *s) {
    // output data
    std::vector<float> outDat;
    for (int i = 0; i < h * w * 3; i++) {
        outDat.push_back(0.0f);
    }
    outBuf = new SSBO<float>(outDat, 0);

    // ray data
    std::vector<GPURay> rayDat;
    for (int i = 0; i < w * h; i++) {
        GPURay empty;
        rayDat.push_back(empty);
    }
    rayBuf = new SSBO<GPURay>(rayDat, 1);

    // sphere data
    std::vector<GPUSphere> spheres;
    std::vector<std::shared_ptr<Object>> objs = s->getObjects();
    for (auto &o : objs) {
        auto oPtr = o.get();
        Sphere *sphereCast = dynamic_cast<Sphere *>(oPtr);
        if (sphereCast != nullptr) {
            GPUSphere sp;
            sp.center = glm::vec4(sphereCast->getCenter(), 1.0f);
            sp.radius = glm::vec4(sphereCast->getRadius(), 0.0f, 0.0f, 0.0f);
            spheres.push_back(sp);
        }
    }

    sphereBuf = new SSBO<GPUSphere>(spheres, 2);

    // triangles and AABB
    std::vector<GPUTriangle> tris;
    std::vector<GPUAABB> boxes;
    for (auto &o : objs) {
        auto oPtr = o.get();
        Mesh *meshCast = dynamic_cast<Mesh *>(oPtr);
        if (meshCast != nullptr) {
            GPUAABB box;
            auto bb = meshCast->getBoundingBox();
            box.minPos = glm::vec4(bb->getMinPos(), 0.0f);
            box.maxPos = glm::vec4(bb->getMaxPos(), 0.0f);
            box.triPos.x = tris.size();
            auto meshTris = meshCast->getTris();
            for (auto &t : meshTris) {
                GPUTriangle currTri;
                auto triArray = t.get()->getPoints();
                currTri.p1 = glm::vec4(triArray.at(0), 1.0f);
                currTri.p2 = glm::vec4(triArray.at(1), 1.0f);
                currTri.p3 = glm::vec4(triArray.at(2), 1.0f);
                tris.push_back(currTri);
            }
            box.triPos.y = tris.size();
            box.triPos.z = 0;
            box.triPos.w = 0;
            boxes.push_back(box);
        }
    }

    triBuf = new SSBO<GPUTriangle>(tris, 3);
    boxBuf = new SSBO<GPUAABB>(boxes, 4);
}

void GPUBackend::render(Scene *s, Display *d) {

    shader->useProgram();

    if (!buffersMade) {
        createBuffers(s);
        buffersMade = true;
    }

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
    rayBuf->modify(rayDat);

    outBuf->bind();
    rayBuf->bind();
    sphereBuf->bind();
    triBuf->bind();
    boxBuf->bind();

    shader->dispatch(w, h);

    auto outDat = outBuf->read();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            d->drawPixel(x, y,
                         glm::vec3(outDat.at((400 * x + y) * 3),
                                   outDat.at((400 * x + y) * 3 + 1),
                                   outDat.at((400 * x + y) * 3 + 2)));
        }
    }
}