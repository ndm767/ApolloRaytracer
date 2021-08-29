#pragma once

#include "Ray.hpp"
#include "Scene.hpp"
#include "camera/PerspCamera.hpp"
#include "display/ASCIIDisplay.hpp"
#include "display/SDLDisplay.hpp"

#include <string>

class ApolloRaytracer {
  public:
    ApolloRaytracer(std::string scenePath, int outWidth, int outHeight,
                    int outRes, int maxRayDepth, int numSamp);
    ~ApolloRaytracer();

    void run();

  protected:
    void handleEvents(std::shared_ptr<Camera> activeCamera);
    void drawColumn(Camera *c, Scene *s, int x);
    void drawPixels(Scene *s);

    int width, height, res;
    int currCam;
    int rayDepth;
    int numSamples;

    Display *output;
    Scene *scene;

    bool shouldUpdate;
};