#include "ScreenTexture.hpp"

ScreenTexture::ScreenTexture(std::vector<std::vector<glm::vec3>> &pixels) {
    w = pixels.size();
    h = pixels.at(0).size();

    data = new unsigned char[h * w * 3];

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            glm::vec3 col = pixels.at(x).at(y);
            data[(y * w + x) * 3] = static_cast<unsigned char>(col.r * 255);
            data[(y * w + x) * 3 + 1] = static_cast<unsigned char>(col.g * 255);
            data[(y * w + x) * 3 + 2] = static_cast<unsigned char>(col.b * 255);
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
}

ScreenTexture::~ScreenTexture() {
    glDeleteTextures(1, &texture);
    delete data;
}

void ScreenTexture::set(std::vector<std::vector<glm::vec3>> &pixels) {
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            glm::vec3 col = pixels.at(x).at(y);
            data[(y * w + x) * 3] = static_cast<unsigned char>(col.r * 255);
            data[(y * w + x) * 3 + 1] = static_cast<unsigned char>(col.g * 255);
            data[(y * w + x) * 3 + 2] = static_cast<unsigned char>(col.b * 255);
        }
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
}

void ScreenTexture::use() { glBindTexture(GL_TEXTURE_2D, texture); }