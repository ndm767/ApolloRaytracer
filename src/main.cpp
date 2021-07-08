#include "Ray.hpp"
#include "display/SDLDisplay.hpp"
#include "geometry/Sphere.hpp"

int main(int argc, char *argv[]) {
    Display *output = new SDLDisplay(400, 400);

    Object *s = new Sphere(glm::vec3(0, 0, 2), 1.0f);

    for (int x = 0; x < 400; x++) {
        for (int y = 0; y < 400; y++) {
            Ray r(
                glm::vec3(2 * x / 400.0f - 1.0f, 2 * y / 400.0f - 1.0f, -1.0f),
                glm::vec3(0, 0, 1));
            output->drawPixel(x, y, r.traceRay(s));
        }
    }

    while (!output->isFinished()) {
        output->flush();
    }

    delete s;
    delete output;
    return 0;
}