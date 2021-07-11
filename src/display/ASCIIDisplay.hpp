#pragma once

#include "display.hpp"

#include <string>

class ASCIIDisplay : public Display {
  public:
    ASCIIDisplay(unsigned width, unsigned height);
    ~ASCIIDisplay();

    void drawPixel(unsigned x, unsigned y, glm::vec3 color) override;
    void flush() override;

    glm::vec3 getPixel(unsigned x, unsigned y) override;

    bool getEventDown(int event) override;
    bool getEventUp(int event) override;

  protected:
    std::string values = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
                         "\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
};