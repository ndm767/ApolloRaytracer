#pragma once

#include "Ray.hpp"
#include "Scene.hpp"
#include "backends/backend.hpp"
#include "camera/PerspCamera.hpp"
#include "display/ASCIIDisplay.hpp"
#include "display/SDLDisplay.hpp"

#include <string>

class ApolloRaytracer {
  public:
    ApolloRaytracer(std::string scenePath, int outWidth, int outHeight,
                    int outRes, int maxRayDepth, int numSamp, bool useCPU);
    ~ApolloRaytracer();

    void run();

  protected:
    void handleEvents(std::shared_ptr<Camera> activeCamera);

    int width, height, res;
    int currCam;
    int rayDepth;
    int numSamples;

    Display *output;
    Scene *scene;

    Backend *backend;

    bool shouldUpdate;
};