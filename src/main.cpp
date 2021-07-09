#include "Ray.hpp"
#include "Scene.hpp"
#include "camera/PerspCamera.hpp"
#include "display/SDLDisplay.hpp"
#include "geometry/Sphere.hpp"
#include "light/Light.hpp"

int main(int argc, char *argv[]) {

    int width = 400;
    int height = 400;

    Display *output = new SDLDisplay(width, height);

    Scene s(width, height, glm::vec3(1, 1, 1), 0.1f);
    s.setIBL("assets/hdr/studio.hdr");
    output->setActiveCamera(s.getActiveCamera());

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

    bool shouldUpdate = true;
    int res = 5;

    while (!output->isFinished()) {
        if (shouldUpdate) {
            for (int x = 0; x < width; x += res) {
                for (int y = 0; y < height; y += res) {
                    Ray r = s.getActiveCamera().get()->getRayAtPixel(x, y);
                    for (int i = 0; i < res; i++) {
                        for (int j = 0; j < res; j++) {
                            output->drawPixel(x + i, y + j, r.traceRay(s));
                        }
                    }
                }
            }
            shouldUpdate = false;
        }

        output->flush(&shouldUpdate);
    }

    delete output;
    return 0;
}