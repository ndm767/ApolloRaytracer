#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include <iostream>

Texture::Texture(std::string path) {
    texData = stbi_load(path.c_str(), &width, &height, &numChannels, 3);

    if (texData == NULL) {
        texFailed = true;
        std::cout << "Could not load texture: " << path << std::endl;
    } else {
        texFailed = false;
    }
}

Texture::~Texture() { stbi_image_free(texData); }

glm::vec3 Texture::getColorAtPos(float u, float v) {
    if (texFailed) {
        return glm::vec3(0, 0, 0);
    }

    int xPos = int(u * width);
    int yPos = int(v * height);

    glm::vec3 ret = glm::vec3(0, 0, 0);

    ret.r = texData[(yPos * width + xPos) * 3] / 255.0f;
    ret.g = texData[(yPos * width + xPos) * 3 + 1] / 255.0f;
    ret.b = texData[(yPos * width + xPos) * 3 + 2] / 255.0f;

    return ret;
}