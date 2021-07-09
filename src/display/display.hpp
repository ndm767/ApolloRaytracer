#pragma once

#include <glm/glm.hpp>
#include <vector>

// display parent class
class Display {
  public:
    Display() {}
    virtual ~Display() {}

    // draw pixel at specified coordinates
    virtual void drawPixel(unsigned x, unsigned y, glm::vec3 color) = 0;
    // update the display/output image
    virtual void flush(bool *shouldUpdate) = 0;

    bool isFinished() { return finished; }
    virtual glm::vec3 getPixel(unsigned x, unsigned y) = 0;

    // get keyboard (or other) events (pretty much only for SDL display)
    virtual bool getEventDown(int event) = 0;

  protected:
    bool finished;
    std::vector<std::vector<glm::vec3>> pixels;
};