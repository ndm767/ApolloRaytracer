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
    s.addObject(
        std::make_shared<Sphere>(glm::vec3(0, 0, 3), 1.0f, glm::vec3(1, 0, 0)));
    s.addObject(std::make_shared<Sphere>(glm::vec3(0.5f, 1.5f, 2), 0.5f,
                                         glm::vec3(0, 1, 0)));

    s.addLight(std::make_shared<Light>(glm::vec3(0, 3, 3), 1.0f));

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