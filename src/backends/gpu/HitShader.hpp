#pragma once

#include <string>

std::string hitShaderSrc = R""(
    #version 430 core
    
    layout(local_size_x = 1, local_size_y = 1) in; 
    
    // uniforms 
    uniform int screenWidth;

    // structs
    struct Ray {
        vec4 orig;
        vec4 dir;
    };

    struct Sphere {
        vec4 pos;
        vec4 r;
        ivec4 matIndex;
    };

    struct Triangle {
        vec4 p1;
        vec4 p2;
        vec4 p3;
        vec4 normal;
        ivec4 matIndex;
    };

    struct AABB{
        vec4 minPos;
        vec4 maxPos;
        ivec4 triPos;
    };

    struct GPURetData{
        vec4 hitPos;
        vec4 hitNorm;
        ivec4 matIndex;
    };

    // buffers
    layout(std430, binding = 0) buffer outBuf
    {
        GPURetData outDat[];
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

    layout(std430, binding = 4) buffer boxBuf
    {
        AABB boxes[];
    };

    // functions
    GPURetData raySphereIntersect(Ray r, Sphere s){
        vec3 rayDir = r.dir.xyz;
        vec3 rayOrig = r.orig.xyz;
        vec3 sPos = s.pos.xyz;
        float radius = s.r.x;

        GPURetData ret;
        ret.matIndex.y = -1;

        float a = dot(rayDir, rayDir);
        float b = 2.0 * dot(rayDir, rayOrig - sPos);
        float c = dot(rayOrig - sPos, rayOrig - sPos) - radius * radius;

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
                vec3 hitPos = rayOrig+rayDir*coef;
                ret.hitPos = vec4(hitPos, 0.0);
                vec3 hitNorm = ret.hitPos.xyz - sPos;
                ret.hitNorm = vec4(hitNorm, 0.0);
                ret.matIndex.y = 1;
                ret.matIndex.x = s.matIndex.x;
                return ret;
            }else{
                return ret;
            }
        }else{
            return ret;
        }
    }

    GPURetData testSpheres(Ray r){
        bool found = false;
        GPURetData ret;
        ret.matIndex.y = -1;

        for(int i = 0; i<spheres.length(); i++){
            GPURetData tempRet = raySphereIntersect(r, spheres[i]);
            if(tempRet.matIndex.y != -1){
                if(!found || distance(tempRet.hitPos.xyz, r.orig.xyz) < distance(ret.hitPos.xyz, r.orig.xyz)){
                    ret = tempRet;
                }
                found = true;
            }
        }

        return ret;
    }

    GPURetData rayTriangleIntersect(Ray r, Triangle tri){
        vec3 p0 = tri.p1.xyz;
        vec3 p1 = tri.p2.xyz;
        vec3 p2 = tri.p3.xyz;

        vec3 rayDir = r.dir.xyz;
        vec3 rayOrig = r.orig.xyz;

        GPURetData ret;
        ret.matIndex.y = -1;


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
            return ret;
        }

        mat3 gMat;
        gMat[0] = vec3(p0.x - p1.x, p0.x - rayOrig.x, rayDir.x);
        gMat[1] = vec3(p0.y - p1.y, p0.y - rayOrig.y, rayDir.y);
        gMat[2] = vec3(p0.z - p1.z, p0.z - rayOrig.z, rayDir.z);

        float gamma = determinant(gMat) / detA;
        if (gamma < 0 || gamma > 1) {
            return ret;
        }

        mat3 bMat;
        bMat[0] = vec3(p0.x - rayOrig.x, p0.x - p2.x, rayDir.x);
        bMat[1] = vec3(p0.y - rayOrig.y, p0.y - p2.y, rayDir.y);
        bMat[2] = vec3(p0.z - rayOrig.z, p0.z - p2.z, rayDir.z);

        float beta = determinant(bMat) / detA;
        if (beta < 0 || beta > 1 - gamma) {
            return ret;
        }

        ret.hitPos = vec4(rayOrig + rayDir * t, 0.0);
        ret.hitNorm = tri.normal;
        ret.matIndex.x = tri.matIndex.x;
        ret.matIndex.y = 1;
        return ret;
    }

    bool rayBoxIntersection(Ray r, AABB b){
        vec3 rayOrig = r.orig.xyz;
        vec3 rayDir = r.dir.xyz;

        vec3 minPos = b.minPos.xyz;
        vec3 maxPos = b.maxPos.xyz;

        if (rayDir.x != 0) {
            float t = 0.0;
            if (rayDir.x > 0) {
                t = (minPos.x - rayOrig.x) / rayDir.x;
            } else {
                t = (maxPos.x - rayOrig.x) / rayDir.x;
            }

            float yPos = rayOrig.y + rayDir.y * t;
            if (minPos.y <= yPos && maxPos.y >= yPos) {
                float zPos = rayOrig.z + rayDir.z * t;
                if (minPos.z <= zPos && maxPos.z >= zPos) {
                    return true;
                }
            }
        }

        if (rayDir.y != 0) {
            float t = 0.0;
            if (rayDir.y > 0) {
                t = (minPos.y - rayOrig.y) / rayDir.y;
            } else {
                t = (maxPos.y - rayOrig.y) / rayDir.y;
            }

            float xPos = rayOrig.x + rayDir.x * t;
            if (minPos.x <= xPos && maxPos.x >= xPos) {
                float zPos = rayOrig.z + rayDir.z * t;
                if (minPos.z <= zPos && maxPos.z >= zPos) {
                    return true;
                }
            }
        }

        if (rayDir.z != 0) {
            float t = 0.0;
            if (rayDir.z > 0) {
                t = (minPos.z - rayOrig.z) / rayDir.z;
            } else {
                t = (maxPos.z - rayOrig.z) / rayDir.z;
            }

            float yPos = rayOrig.y + rayDir.y * t;
            if (minPos.y <= yPos && maxPos.y >= yPos) {
                float xPos = rayOrig.x + rayDir.x * t;
                if (minPos.x <= xPos && maxPos.x >= xPos) {
                    return true;
                }
            }
        }

        return false;
    }

    GPURetData testMeshes(Ray r){
        bool found = false;
        GPURetData ret;
        ret.matIndex.y = -1;

        for(int i = 0; i<boxes.length(); i++){
            if(rayBoxIntersection(r, boxes[i])){
                for(int j = boxes[i].triPos.x; j<boxes[i].triPos.y; j++){
                    GPURetData tempRet = rayTriangleIntersect(r, tris[j]);
                    if(tempRet.matIndex.y != -1){
                        if(!found || distance(tempRet.hitPos.xyz, r.orig.xyz) < distance(ret.hitPos.xyz, r.orig.xyz)){
                            ret = tempRet;
                        }
                        found = true;
                    }
                }
            }
        }

        return ret;
    }

    void main(){
        ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

        int loc = (coords.x*screenWidth + coords.y);

        GPURetData sphereRet = testSpheres(rays[loc]);
        GPURetData meshRet = testMeshes(rays[loc]);

        bool sphereFound = true;
        if(sphereRet.matIndex.y == -1){
            sphereFound = false;
        }

        bool meshFound = true;
        if(meshRet.matIndex.y == -1){
            meshFound = false;
        }

        vec3 outCol;
        if(sphereFound && meshFound){
            if(distance(sphereRet.hitPos.xyz, rays[loc].orig.xyz) < distance(meshRet.hitPos.xyz, rays[loc].orig.xyz)){
                outDat[loc] = sphereRet;
            }else{
                outDat[loc] = meshRet;
            }
        }else if(sphereFound){
            outDat[loc] = sphereRet;
        }else if(meshFound){
            outDat[loc] = meshRet;
        }else{
            outDat[loc].matIndex.y = -1;
            outDat[loc].hitPos = rays[loc].dir;
        }

    }
)"";