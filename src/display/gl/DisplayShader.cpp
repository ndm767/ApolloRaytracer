#include "DisplayShader.hpp"

#include <iostream>
#include <string>

DisplayShader::DisplayShader() {
    std::string vertStr = R""(
        #version 430 core

        in vec3 pos;
        in vec2 tPos;

        out vec2 texPos;

        void main(){
            gl_Position = vec4(pos, 1.0);
            texPos = tPos;
        }
    )"";

    std::string fragStr = R""(
        #version 430 core

        in vec2 texPos;

        out vec4 outColor;

        uniform sampler2D tex;

        void main(){
            outColor = texture(tex, texPos);
        }
    )"";

    const char *vertSrc = vertStr.c_str();
    const char *fragSrc = fragStr.c_str();

    GLuint vertShader, fragShader;
    int success;
    char infoLog[512];

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, NULL);
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "quad vertex shader error: " << infoLog << std::endl;
    }

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "quad fragment shader error: " << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "quad shader program error: " << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

DisplayShader::~DisplayShader() { glDeleteProgram(program); }

void DisplayShader::use() { glUseProgram(program); }