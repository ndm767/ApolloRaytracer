#pragma once

#include "FileLoader.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "camera/PerspCamera.hpp"
#include "display/ASCIIDisplay.hpp"
#include "display/SDLDisplay.hpp"
#include "geometry/Sphere.hpp"
#include "light/Light.hpp"

class ApolloRaytracer {
  public:
    ApolloRaytracer(int outWidth, int outHeight, int outRes);
    ~ApolloRaytracer();

    void run();

  protected:
    void handleEvents(std::shared_ptr<Camera> activeCamera);
    void drawColumn(Camera *c, Scene *s, int x);
    void drawPixels(Scene *s);

    int width, height, res;
    int currCam;

    Display *output;
    Scene *scene;

    bool shouldUpdate;
};