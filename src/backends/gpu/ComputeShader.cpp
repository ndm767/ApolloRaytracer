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

void ComputeShader::createSSBO(std::vector<float> data, int binding) {
    // add binding to list of ssbos
    GLuint SSBO;
    ssbos.push_back(std::make_pair(binding, SSBO));
    int ssboPos = ssbos.size() - 1;

    // create buffer and add data
    glGenBuffers(1, &ssbos[ssboPos].second);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbos[ssboPos].second);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(float),
                 data.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbos[ssboPos].second);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::vector<float> ComputeShader::readSSBO(int binding, int size) {

    for (auto &b : ssbos) {
        if (b.first == binding) {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, b.second);
            break;
        }
    }

    std::vector<float> ret(size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(float),
                       ret.data());

    return ret;
}

void ComputeShader::useProgram() { glUseProgram(program); }

void ComputeShader::dispatch(int width, int height) {
    glDispatchCompute((GLuint)width, (GLuint)height, 1);

    glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
}