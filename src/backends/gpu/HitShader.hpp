#pragma once

#include <string>

std::string hitShaderSrc = R""(
    #version 430
    
    layout(local_size_x = 1, local_size_y = 1) in; 

    layout(std430, binding = 0) buffer testBuf 
    {
        float dat[];
    };

    layout(std430, binding = 1) buffer outBuf
    {
        float outDat[];
    };

    void main(){
        ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

        outDat[(coords.y*400 + coords.x)*3] = dat[0];
        outDat[(coords.y*400 + coords.x)*3 + 1] = dat[1];
        outDat[(coords.y*400 + coords.x)*3 + 2] = dat[2];

    }
)"";