#include "display/SDLDisplay.hpp"

int main(int argc, char *argv[]) {
    Display *output = new SDLDisplay(400, 400);

    for (int x = 0; x < 400; x++) {
        for (int y = 0; y < 400; y++) {
            output->drawPixel(x, y, glm::vec3(x / 400.0f, y / 400.0f, 0.0f));
        }
    }

    while (!output->isFinished()) {
        output->flush();
    }

    delete output;
    return 0;
}