#pragma once

#include "backend.hpp"

class CPUBackend : public Backend {
  public:
    CPUBackend(int width, int height, int numSamples, int resolution);
    ~CPUBackend();

    void render(Scene *s, Display *d) override;

  protected:
    void drawPixels(Display *d, Scene *s);
    void drawColumn(Display *d, Camera *c, Scene *s, int x);
};