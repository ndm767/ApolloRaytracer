#include "GPUBackend.hpp"

#include "gpu/GPUTypes.hpp"
#include "gpu/HitShader.hpp"

#include "../Ray.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Sphere.hpp"

#include <GL/glew.h>

#include <future>
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
    // delete previous buffers if we are recreating them
    if (buffersMade) {
        delete triBuf;
        delete boxBuf;
        delete sphereBuf;
        delete rayBuf;
    }
    // output data
    std::vector<GPURetData> outDat;
    for (int i = 0; i < h * w; i++) {
        GPURetData empty;
        outDat.push_back(empty);
    }
    outBuf = new SSBO<GPURetData>(outDat, 0);

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
            sp.matIndex.x = mats.size();
            mats.push_back(sphereCast->getMaterial());
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
            /** CHANGE THIS FOR MULTIPLE MATERIALS PER MESH **/
            int matInd = mats.size();
            mats.push_back(meshCast->getMaterial());
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
                currTri.normal = glm::vec4(t.get()->getNorm(), 1.0f);
                currTri.matIndex.x = matInd;
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

    // leaving this open to potential scene changes in the future
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

    std::vector<std::future<void>> threads;

    for (int x = 0; x < w; x++) {
        threads.push_back(
            std::async(&GPUBackend::drawColumn, this, x, s, d, outDat));
    }

    for (auto &t : threads) {
        t.wait();
    }
}

void GPUBackend::drawColumn(int x, Scene *s, Display *d,
                            std::vector<GPURetData> outDat) {
    for (int y = 0; y < h; y++) {
        glm::vec3 color = getHitColor(outDat.at(400 * x + y), s);
        d->drawPixel(x, y, color);
    }
}

glm::vec3 GPUBackend::getHitColor(GPURetData hitData, Scene *s) {
    if (hitData.matIndex.y != -1) {
        return mats.at(hitData.matIndex.x).getDiffuse();
    } else {
        if (s->getUseEnv()) {
            return s->getEnvColor(glm::vec3(hitData.hitPos.x, hitData.hitPos.y,
                                            hitData.hitPos.z));
        } else {
            return glm::vec3(0.0f);
        }
    }
}