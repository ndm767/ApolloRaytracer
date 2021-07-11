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

    bool getEventDown(int event) override;
    bool getEventUp(int event) override;

  protected:
    SDL_Window *rWindow;
    SDL_Renderer *rRenderer;

    // sees if a key is in a vector
    bool inVector(std::vector<SDL_Scancode> &vec, SDL_Scancode key);
    // sets key as pressed
    void setKeyDown(SDL_KeyboardEvent ke);
    // removes keys that are no longer pressed
    void cleanKeys();
    std::vector<SDL_Scancode> keysDown;
    std::vector<SDL_Scancode> keysUp;
};