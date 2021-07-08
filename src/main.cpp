#include "Ray.hpp"
#include "camera/PerspCamera.hpp"
#include "display/SDLDisplay.hpp"
#include "geometry/Sphere.hpp"

int main(int argc, char *argv[]) {
    Display *output = new SDLDisplay(400, 400);

    Camera *cam = new PerspCamera(400, 400, glm::vec3(0, 0, 0),
                                  glm::vec3(0, 0, 1), 60.0f);
    Object *s = new Sphere(glm::vec3(0, 0, 3), 1.0f);

    for (int x = 0; x < 400; x++) {
        for (int y = 0; y < 400; y++) {
            Ray r = cam->getRayAtPixel(x, y);
            output->drawPixel(x, y, r.traceRay(s));
        }
    }

    while (!output->isFinished()) {
        output->flush();
    }

    delete s;
    delete cam;
    delete output;
    return 0;
}