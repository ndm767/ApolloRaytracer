#pragma once

#include "../Scene.hpp"
#include "../display/display.hpp"

// backend parent class
class Backend {
  public:
    Backend() {}
    virtual ~Backend() {}

    virtual void render(Scene *s, Display *d) = 0;

  protected:
    int numSamp;
    int res;
    int w, h;
};