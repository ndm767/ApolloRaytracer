#include "ASCIIDisplay.hpp"

#include <iostream>

ASCIIDisplay::ASCIIDisplay(unsigned width, unsigned height) {
    finished = false;

    // set display to black to start
    for (int y = 0; y < height; y++) {
        std::vector<glm::vec3> row;
        for (int x = 0; x < width; x++) {
            row.push_back(glm::vec3(0, 0, 0));
        }
        pixels.push_back(row);
    }
}

ASCIIDisplay::~ASCIIDisplay() {}

void ASCIIDisplay::drawPixel(unsigned x, unsigned y, glm::vec3 color) {
    if (y < pixels.size() && x < pixels.at(y).size()) {
        pixels.at(y).at(x) = color;
    }
}

void ASCIIDisplay::flush() {

    for (auto &y : pixels) {
        std::cout << "\n";
        for (auto x : y) {
            float val = x.r + x.g + x.b;
            val /= 3.0f;
            val *= 9.0f;

            int valIndex = int(val);

            std::cout << values.at(valIndex);
        }
    }

    std::cout << std::endl;

    // we don't want to repeatedly print because there are no events
    finished = true;
}

glm::vec3 ASCIIDisplay::getPixel(unsigned x, unsigned y) {
    if (y < pixels.size() && x < pixels.at(y).size()) {
        return pixels.at(y).at(x);
    }

    return glm::vec3(0);
}

bool ASCIIDisplay::getEventDown(int event) {
    // ascii display doesn't have events (so far, maybe text-adventure style
    // inputs?)
    return false;
}