#include "QuadRenderer.hpp"

QuadRenderer::QuadRenderer() {

    // clang-format off
    float verts[] = {
        -1.0f, -1.0f, 0.0f, 
        -1.0f, 1.0f,  0.0f, 
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 
        1.0f, -1.0f, 0.0f, 
        1.0f, 1.0f,  0.0f
    };

    float texVerts[] = {
        0.0f, 1.0f, 
        0.0f, 0.0f, 
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    // clang-format on

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), texVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
}

QuadRenderer::~QuadRenderer() {
    glDeleteBuffers(1, &texVBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void QuadRenderer::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}