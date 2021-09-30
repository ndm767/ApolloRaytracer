#include "ApolloRaytracer.hpp"

#include <iostream>

#include "SceneLoader.hpp"
#include "backends/CPUBackend.hpp"
#include "backends/GPUBackend.hpp"

ApolloRaytracer::ApolloRaytracer(std::string scenePath, int outWidth,
                                 int outHeight, int outRes, int maxRayDepth,
                                 int numSamp, bool useCPU) {

    width = outWidth;
    height = outHeight;
    shouldUpdate = true;
    res = outRes;
    currCam = 0;
    numSamples = numSamp;

    rayDepth = maxRayDepth;

    output = new SDLDisplay(width, height);
    // output = new ASCIIDisplay(width, height);

    scene = new Scene(width, height, rayDepth, glm::vec3(1, 1, 1), 0.1f);

    if (useCPU) {
        backend = new CPUBackend(width, height, numSamples, res);
    } else {
        backend = new GPUBackend(width, height, numSamp, res);
    }

    SceneLoader sl;
    sl.loadScene(width, height, rayDepth, scenePath, *scene, !useCPU);

    numFrames = 0;
    totalTime = 0;
}

ApolloRaytracer::~ApolloRaytracer() {
    delete scene;
    delete output;
    delete backend;
}

void ApolloRaytracer::run() {
    while (!output->isFinished()) {
        if (shouldUpdate) {
            Uint32 sTime = SDL_GetTicks();
            backend->render(scene, output);
            Uint32 eTime = SDL_GetTicks();
            // std::cout << "Frame time: " << eTime - sTime << std::endl;
            shouldUpdate = false;
            totalTime += (eTime - sTime);
            numFrames += 1;
        }

        output->flush();
        handleEvents(scene->getActiveCamera());
        if (output->getEventUp(SDL_SCANCODE_SPACE)) {
            shouldUpdate = true;
            currCam++;
            if (currCam >= scene->getNumCameras()) {
                currCam = 0;
            }
            scene->setActiveCamera(currCam);
        }
    }

    std::cout << "Average Frame Time: " << totalTime / numFrames << std::endl;
}

// handles events for when the display is an SDLDisplay
void ApolloRaytracer::handleEvents(std::shared_ptr<Camera> activeCamera) {
    float speed = 0.25f;
    float rotSpeed = 5.0f;

    // vertical movement
    if (output->getEventDown(SDL_SCANCODE_Q)) {
        shouldUpdate = true;
        activeCamera.get()->translate(glm::vec3(0, -speed, 0));
    }
    if (output->getEventDown(SDL_SCANCODE_E)) {
        shouldUpdate = true;
        activeCamera.get()->translate(glm::vec3(0, speed, 0));
    }
    // vertical rotation
    if (output->getEventDown(SDL_SCANCODE_UP)) {
        shouldUpdate = true;
        activeCamera.get()->rotate(-rotSpeed, 0.0f);
    }
    if (output->getEventDown(SDL_SCANCODE_DOWN)) {
        shouldUpdate = true;
        activeCamera.get()->rotate(rotSpeed, 0.0f);
    }
    // horizontal rotation
    if (output->getEventDown(SDL_SCANCODE_LEFT)) {
        shouldUpdate = true;
        activeCamera.get()->rotate(0.0f, -rotSpeed);
    }
    if (output->getEventDown(SDL_SCANCODE_RIGHT)) {
        shouldUpdate = true;
        activeCamera.get()->rotate(0.0f, rotSpeed);
    }
    // forward and backward movement
    if (output->getEventDown(SDL_SCANCODE_W)) {
        shouldUpdate = true;
        activeCamera.get()->translate(speed * activeCamera.get()->getDir());
    }
    if (output->getEventDown(SDL_SCANCODE_S)) {
        shouldUpdate = true;
        activeCamera.get()->translate(-speed * activeCamera.get()->getDir());
    }
    // left and right movement (works by getting the perpendicular vector to
    // where the camera is facing)
    if (output->getEventDown(SDL_SCANCODE_A)) {
        shouldUpdate = true;
        activeCamera.get()->translate(speed * activeCamera.get()->getPerpDir());
    }
    if (output->getEventDown(SDL_SCANCODE_D)) {
        shouldUpdate = true;
        activeCamera.get()->translate(-speed *
                                      activeCamera.get()->getPerpDir());
    }
}
