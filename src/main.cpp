#include "Ray.hpp"
#include "Scene.hpp"
#include "camera/PerspCamera.hpp"
#include "display/SDLDisplay.hpp"
#include "geometry/Sphere.hpp"
#include "light/Light.hpp"

int main(int argc, char *argv[]) {
    Display *output = new SDLDisplay(400, 400);

    Camera *cam = new PerspCamera(400, 400, glm::vec3(0, 1, 0),
                                  glm::vec3(0, 0, 1), 60.0f);
    Scene s(400, 400, glm::vec3(1, 1, 1), 0.1f);

    Material red(glm::vec3(1, 0, 0), glm::vec3(0.5f), 100);
    Material grey(glm::vec3(0.25f), glm::vec3(0.25f), 10);
    Material green(glm::vec3(0, 1, 0), glm::vec3(0.5f), 10);
    red.setUseReflection(true);
    red.setReflectionCoef(0.75f);
    grey.setUseReflection(true);
    grey.setReflectionCoef(0.25f);

    s.addObject(std::make_shared<Sphere>(glm::vec3(0, 0, 5), 1.0f, red));
    s.addObject(std::make_shared<Sphere>(glm::vec3(0, -101, 5), 100.0f, grey));
    s.addObject(std::make_shared<Sphere>(glm::vec3(2, -0.5f, 4), 0.5f, green));

    s.addLight(std::make_shared<Light>(glm::vec3(0, 2, 4), 1.0f));
    s.addLight(std::make_shared<Light>(glm::vec3(-2, 2, 2), 0.5f));

    for (int x = 0; x < 400; x++) {
        for (int y = 0; y < 400; y++) {
            Ray r = cam->getRayAtPixel(x, y);
            output->drawPixel(x, y, r.traceRay(s));
        }
    }

    while (!output->isFinished()) {
        output->flush();
    }

    delete cam;
    delete output;
    return 0;
}