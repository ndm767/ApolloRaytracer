#pragma once

#include <string>

std::string hitShaderSrc = R""(
    #version 430 core
    
    layout(local_size_x = 1, local_size_y = 1) in; 
    
    // structs
    struct Ray {
        vec4 orig;
        vec4 dir;
    };

    struct Sphere {
        vec4 pos;
        float r;
    };

    struct Triangle {
        vec4 p1;
        vec4 p2;
        vec4 p3;
    };

    // buffers
    layout(std430, binding = 0) buffer outBuf
    {
        float outDat[];
    };

    layout(std430, binding = 1) buffer rayBuf
    {
        Ray rays[];
    };

    layout(std430, binding = 2) buffer sphereBuf
    {
        Sphere spheres[];
    };

    layout(std430, binding = 3) buffer triBuf
    {
        Triangle tris[];
    };

    // functions
    vec3 raySphereIntersect(Ray r, Sphere s){
        vec3 rayDir = r.dir.xyz;
        vec3 rayOrig = r.orig.xyz;
        vec3 sPos = s.pos.xyz;

        float a = dot(rayDir, rayDir);
        float b = 2.0 * dot(rayDir, rayOrig - sPos);
        float c = dot(rayOrig - sPos, rayOrig - sPos) - s.r * s.r;

        float disc = b*b - 4 * a * c;
        if(disc > 0){
            float quadTop = -b - sqrt(disc);
            float coef = 0.0;
            if(quadTop > 0.0){
                coef = quadTop;
            }else{
                quadTop = -b + sqrt(disc);
                coef = quadTop;
            }
            
            coef /= (2.0 * a);

            float epsilon = pow(2.0, -52.0);
            if(coef > epsilon){
                return rayOrig + rayDir * coef;
            }else{
                return vec3(-1.0);
            }
        }else{
            return vec3(-1.0);
        }
    }

    vec3 testSpheres(Ray r){
        bool found = false;
        vec3 hitPos;

        for(int i = 0; i<spheres.length(); i++){
            vec3 tempHitPos = raySphereIntersect(r, spheres[i]);
            if(tempHitPos != vec3(-1.0)){
                found = true;
            }
        }

        if(found){
            return vec3(1, 0, 0);
        }

        return vec3(0.0);
    }

    vec3 rayTriangleIntersect(Ray r, Triangle tri){
        vec3 p0 = tri.p1.xyz;
        vec3 p1 = tri.p2.xyz;
        vec3 p2 = tri.p3.xyz;

        vec3 rayDir = r.dir.xyz;
        vec3 rayOrig = r.orig.xyz;


        // a mat
        mat3 aMat;
        aMat[0] = vec3(p0.x - p1.x, p0.x - p2.x, rayDir.x);
        aMat[1] = vec3(p0.y - p1.y, p0.y - p2.y, rayDir.y);
        aMat[2] = vec3(p0.z - p1.z, p0.z - p2.z, rayDir.z);

        float detA = determinant(aMat);

        mat3 tMat;
        tMat[0] = vec3(p0.x - p1.x, p0.x - p2.x, p0.x - rayOrig.x);
        tMat[1] = vec3(p0.y - p1.y, p0.y - p2.y, p0.y - rayOrig.y);
        tMat[2] = vec3(p0.z - p1.z, p0.z - p2.z, p0.z - rayOrig.z);

        float t = determinant(tMat) / detA;
        if (t < 0) {
            return vec3(-1.0);
        }

        mat3 gMat;
        gMat[0] = vec3(p0.x - p1.x, p0.x - rayOrig.x, rayDir.x);
        gMat[1] = vec3(p0.y - p1.y, p0.y - rayOrig.y, rayDir.y);
        gMat[2] = vec3(p0.z - p1.z, p0.z - rayOrig.z, rayDir.z);

        float gamma = determinant(gMat) / detA;
        if (gamma < 0 || gamma > 1) {
            return vec3(-1.0);
        }

        mat3 bMat;
        bMat[0] = vec3(p0.x - rayOrig.x, p0.x - p2.x, rayDir.x);
        bMat[1] = vec3(p0.y - rayOrig.y, p0.y - p2.y, rayDir.y);
        bMat[2] = vec3(p0.z - rayOrig.z, p0.z - p2.z, rayDir.z);

        float beta = determinant(bMat) / detA;
        if (beta < 0 || beta > 1 - gamma) {
            return vec3(-1.0);
        }

        return (rayOrig + rayDir * t);
    }

    vec3 testTris(Ray r){
        bool found = false;
        vec3 hitPos;

        for(int i = 0; i<tris.length(); i++){
            vec3 tempHitPos = rayTriangleIntersect(r, tris[i]);
            if(tempHitPos != vec3(-1.0)){
                found = true;
            }
        }

        if(found){
            return vec3(0, 1, 0);
        }

        return vec3(0.0);
    }

    void main(){
        ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

        int loc = (coords.x*400 + coords.y);

        vec3 outCol = testSpheres(rays[loc]);
        outCol += testTris(rays[loc]);

        outDat[loc*3] = outCol.x;
        outDat[loc*3 + 1] = outCol.y;
        outDat[loc*3 + 2] = outCol.z;

    }
)"";