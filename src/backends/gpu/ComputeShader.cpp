#include "ComputeShader.hpp"

#include <iostream>

ComputeShader::ComputeShader(std::string compSrc) {
    const char *shaderSrc = compSrc.c_str();

    int success;
    char infoLog[512];

    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Compute shader error: " << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Shader program error: " << infoLog << std::endl;
    }

    glDeleteShader(shader);
}

ComputeShader::~ComputeShader() { glDeleteProgram(program); }

void ComputeShader::useProgram() { glUseProgram(program); }

void ComputeShader::dispatch(int width, int height) {
    glDispatchCompute((GLuint)width, (GLuint)height, 1);

    glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
}

void ComputeShader::setUniform1i(std::string name, int value) {
    GLuint uniLoc = glGetUniformLocation(program, name.c_str());
    glUniform1i(uniLoc, value);
}