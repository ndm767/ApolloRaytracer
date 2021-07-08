#pragma once

#include "display.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// display that outputs to an sdl2 window
class SDLDisplay : public Display {
  public:
    SDLDisplay(unsigned width, unsigned height);
    ~SDLDisplay();

    void drawPixel(unsigned x, unsigned y, glm::vec3 color) override;
    void flush() override;

    glm::vec3 getPixel(unsigned x, unsigned y) override;

  protected:
    SDL_Window *rWindow;
    SDL_Renderer *rRenderer;
};