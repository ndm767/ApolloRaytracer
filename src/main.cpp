#include "FileLoader.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "camera/PerspCamera.hpp"
#include "display/SDLDisplay.hpp"
#include "geometry/Sphere.hpp"
#include "light/Light.hpp"

#include <future>
#include <iostream>

// handles events for when the display is an SDLDisplay
void handleEvents(Display *d, std::shared_ptr<Camera> activeCamera,
                  bool *shouldUpdate) {
    float speed = 0.25f;
    float rotSpeed = 5.0f;

    // vertical movement
    if (d->getEventDown(SDL_SCANCODE_Q)) {
        *shouldUpdate = true;
        activeCamera.get()->translate(glm::vec3(0, -speed, 0));
    }
    if (d->getEventDown(SDL_SCANCODE_E)) {
        *shouldUpdate = true;
        activeCamera.get()->translate(glm::vec3(0, speed, 0));
    }
    // vertical rotation
    if (d->getEventDown(SDL_SCANCODE_UP)) {
        *shouldUpdate = true;
        activeCamera.get()->rotate(-rotSpeed, 0.0f);
    }
    if (d->getEventDown(SDL_SCANCODE_DOWN)) {
        *shouldUpdate = true;
        activeCamera.get()->rotate(rotSpeed, 0.0f);
    }
    // horizontal rotation
    if (d->getEventDown(SDL_SCANCODE_LEFT)) {
        *shouldUpdate = true;
        activeCamera.get()->rotate(0.0f, -rotSpeed);
    }
    if (d->getEventDown(SDL_SCANCODE_RIGHT)) {
        *shouldUpdate = true;
        activeCamera.get()->rotate(0.0f, rotSpeed);
    }
    // forward and backward movement
    if (d->getEventDown(SDL_SCANCODE_W)) {
        *shouldUpdate = true;
        activeCamera.get()->translate(speed * activeCamera.get()->getDir());
    }
    if (d->getEventDown(SDL_SCANCODE_S)) {
        *shouldUpdate = true;
        activeCamera.get()->translate(-speed * activeCamera.get()->getDir());
    }
    // left and right movement (works by getting the perpendicular vector to
    // where the camera is facing)
    if (d->getEventDown(SDL_SCANCODE_A)) {
        *shouldUpdate = true;
        activeCamera.get()->translate(speed * activeCamera.get()->getPerpDir());
    }
    if (d->getEventDown(SDL_SCANCODE_D)) {
        *shouldUpdate = true;
        activeCamera.get()->translate(-speed *
                                      activeCamera.get()->getPerpDir());
    }
}

// traces rays one column at a time
void drawColumn(Display *output, Camera *c, Scene *s, int x, int height,
                int res) {
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
void drawPixels(Display *output, Scene *s, int width, int height, int res) {
    Camera *currCam = s->getActiveCamera().get();
    std::vector<std::future<void>> threads;

    // makes each column of pixels its own thread
    for (int x = 0; x < width; x += res) {
        threads.push_back(
            std::async(drawColumn, output, currCam, s, x, height, res));
    }

    for (auto &t : threads) {
        t.wait();
    }
}

int main(int argc, char *argv[]) {

    int width = 400;
    int height = 400;

    Display *output = new SDLDisplay(width, height);

    Scene s(width, height, glm::vec3(1, 1, 1), 0.1f);
    s.setIBL("assets/hdr/sunrise.hdr");

    int newCam = s.addCamera(std::make_shared<PerspCamera>(
        width, height, glm::vec3(6, 0, 0), glm::vec3(-1, 0, 0), 60.0f));
    // int newCam = s.addCamera(std::make_shared<PerspCamera>(
    //    width, height, glm::vec3(0, 6, 0), glm::vec3(0, -1, 0), 60.0f));
    // s.setActiveCamera(newCam);

    Material red(glm::vec3(1, 0, 0), glm::vec3(0.5f), 100);
    Material grey(glm::vec3(0.25f), glm::vec3(0.25f), 10);
    Material green(glm::vec3(0, 1, 0), glm::vec3(0.5f), 10);
    red.setUseReflection(true);
    red.setReflectionCoef(0.5f);
    grey.setUseReflection(true);
    grey.setReflectionCoef(0.25f);

    s.addObject(std::make_shared<Sphere>(glm::vec3(0, -101, 5), 100.0f, grey));
    s.addObject(std::make_shared<Sphere>(glm::vec3(2, -0.5f, 4), 0.5f, green));

    FileLoader fl;
    fl.loadFile("assets/models/cow.obj", s, glm::vec3(0, 0, 3), 0.25f, &red);
    fl.loadFile("assets/models/bun_zipper_res4.ply", s,
                glm::vec3(-2, -1.334, 3), 10.0f, &grey);

    s.addLight(std::make_shared<Light>(glm::vec3(0, 3, 4), 1.0f));
    s.addLight(std::make_shared<Light>(glm::vec3(-2, 3, 2), 0.5f));

    bool shouldUpdate = true;
    int res = 2;

    while (!output->isFinished()) {
        if (shouldUpdate) {
            Uint32 sTime = SDL_GetTicks();
            drawPixels(output, &s, width, height, res);
            Uint32 eTime = SDL_GetTicks();
            std::cout << "Frame time: " << eTime - sTime << std::endl;
            shouldUpdate = false;
        }

        output->flush();
        handleEvents(output, s.getActiveCamera(), &shouldUpdate);
        if (output->getEventDown(SDL_SCANCODE_SPACE)) {
            shouldUpdate = true;
            s.setActiveCamera(1);
        } else if (output->getEventDown(SDL_SCANCODE_LCTRL)) {
            shouldUpdate = true;
            s.setActiveCamera(0);
        }
    }

    delete output;
    return 0;
}