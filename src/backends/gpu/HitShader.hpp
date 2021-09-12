#pragma once

#include <string>

std::string hitShaderSrc = R""(
    #version 430 core
    
    layout(local_size_x = 1, local_size_y = 1) in; 

    struct Ray {
        vec4 orig;
        vec4 dir;
    };

    layout(std430, binding = 0) buffer testBuf 
    {
        Ray rays[];
    };

    layout(std430, binding = 1) buffer outBuf
    {
        float outDat[];
    };

    void main(){
        ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

        int loc = (coords.y*400 + coords.x);

        outDat[loc*3] = rays[loc].dir.x;
        outDat[loc*3 + 1] = rays[loc].dir.y;
        outDat[loc*3 + 2] = rays[loc].dir.z;

    }
)"";