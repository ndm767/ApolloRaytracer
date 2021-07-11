#include "ApolloRaytracer.hpp"

#include <future>
#include <iostream>

ApolloRaytracer::ApolloRaytracer(int outWidth, int outHeight, int outRes) {

    width = outWidth;
    height = outHeight;
    shouldUpdate = true;
    res = outRes;
    currCam = 0;

    output = new SDLDisplay(width, height);
    // Display *output = new ASCIIDisplay(width, height);

    scene = new Scene(width, height, glm::vec3(1, 1, 1), 0.1f);
    scene->setEnv("assets/hdr/sunrise.hdr");

    Material red(glm::vec3(1, 0, 0), glm::vec3(0.5f), 100);
    Material grey(glm::vec3(0.25f), glm::vec3(0.25f), 10);
    Material glass(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f), 100);
    red.setUseReflection(true);
    red.setReflectionCoef(0.5f);
    grey.setUseReflection(true);
    grey.setReflectionCoef(0.25f);

    glass.setUseRefraction(true);
    glass.setIndexOfRefraction(1.52f);
    glass.setRefractionCoef(0.99f);

    scene->addObject(
        std::make_shared<Sphere>(glm::vec3(0, -101, 5), 100.0f, grey));
    scene->addObject(
        std::make_shared<Sphere>(glm::vec3(2, -0.5f, 4), 0.5f, grey));

    FileLoader fl;
    fl.loadFile("assets/models/cow.obj", *scene, glm::vec3(0, 0, 3), 0.25f,
                &red);
    fl.loadFile("assets/models/bun_zipper_res4.ply", *scene,
                glm::vec3(-2, -1.334, 3), 10.0f, &glass);

    scene->addLight(std::make_shared<Light>(glm::vec3(0, 3, 4), 1.0f));
    scene->addLight(std::make_shared<Light>(glm::vec3(-2, 3, 2), 0.5f));
}

ApolloRaytracer::~ApolloRaytracer() {
    delete scene;
    delete output;
}

void ApolloRaytracer::run() {
    while (!output->isFinished()) {
        if (shouldUpdate) {
            Uint32 sTime = SDL_GetTicks();
            drawPixels(scene);
            Uint32 eTime = SDL_GetTicks();
            std::cout << "Frame time: " << eTime - sTime << std::endl;
            shouldUpdate = false;
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
}

// traces rays one column at a time
void ApolloRaytracer::drawColumn(Camera *c, Scene *s, int x) {
    for (int y = 0; y < height; y += res) {
        Ray r = c->getRayAtPixel(x, y);
        glm::vec3 result = r.traceRay(*s);
        for (int i = 0; i < res; i++) {
            for (int j = 0; j < res; j++) {
                output->drawPixel(x + i, y + j, result);
            }
        }
    }
}

// traces all the rays and draws the pixels
void ApolloRaytracer::drawPixels(Scene *s) {
    Camera *currCam = s->getActiveCamera().get();
    std::vector<std::future<void>> threads;

    // makes each column of pixels its own thread
    for (int x = 0; x < width; x += res) {
        threads.push_back(
            std::async(&ApolloRaytracer::drawColumn, this, currCam, s, x));
    }

    for (auto &t : threads) {
        t.wait();
    }
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
