#include "SDLDisplay.hpp"

#include <iostream>

SDLDisplay::SDLDisplay(unsigned width, unsigned height) {
    finished = false;

    // set display to black
    for (unsigned x = 0; x < width; x++) {
        std::vector<glm::vec3> column;
        for (unsigned y = 0; y < height; y++) {
            column.push_back(glm::vec3(0, 0, 0));
        }
        pixels.push_back(column);
    }

    SDL_Init(SDL_INIT_VIDEO);
    rWindow = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, width, height,
                               SDL_WINDOW_SHOWN);
    rRenderer = SDL_CreateRenderer(rWindow, -1, SDL_RENDERER_ACCELERATED);
}

SDLDisplay::~SDLDisplay() {
    SDL_DestroyRenderer(rRenderer);
    SDL_DestroyWindow(rWindow);
    SDL_Quit();
}

void SDLDisplay::drawPixel(unsigned x, unsigned y, glm::vec3 color) {
    if (x < pixels.size() && y < pixels.at(0).size()) {
        pixels.at(x).at(y) = color;
    }
}

void SDLDisplay::flush() {
    SDL_SetRenderDrawColor(rRenderer, 0, 0, 0, 255);
    SDL_RenderClear(rRenderer);

    for (unsigned x = 0; x < pixels.size(); x++) {
        for (unsigned y = 0; y < pixels.at(x).size(); y++) {
            glm::vec3 col = pixels.at(x).at(y);
            SDL_SetRenderDrawColor(rRenderer, Uint8(col.r * 255),
                                   Uint8(col.g * 255), Uint8(col.b * 255), 255);
            SDL_RenderDrawPoint(rRenderer, x, y);
        }
    }

    SDL_RenderPresent(rRenderer);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            finished = true;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                finished = true;
            }
        }
    }
}

glm::vec3 SDLDisplay::getPixel(unsigned x, unsigned y) {
    if (x < pixels.size() && y < pixels.at(0).size()) {
        return pixels.at(x).at(y);
    }
    return glm::vec3(0, 0, 0);
}