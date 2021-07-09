#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

Mesh::Mesh(std::string path, glm::vec3 meshPos, float scale, Material mat) {
    objMat = mat;

    Assimp::Importer imp;
    const aiScene *scene = imp.ReadFile(
        path.c_str(),
        aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GenNormals |
            aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded |
            aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices |
            aiProcess_FindDegenerates | aiProcess_FindInvalidData);

    if (std::string(imp.GetErrorString()).compare("") != 0) {
        std::cout << "Assimp error on file: " << path
                  << "\n Error: " << imp.GetErrorString() << std::endl;
    }

    for (unsigned m = 0; m < scene->mNumMeshes; m++) {
        aiMesh *currMesh = scene->mMeshes[m];
        std::vector<glm::vec3> verts;
        std::vector<glm::vec3> normals;
        std::vector<unsigned> indices;
        std::cout << "Mesh has " << currMesh->mNumFaces << " faces"
                  << std::endl;

        // get vertices and vertex normals
        for (unsigned v = 0; v < currMesh->mNumVertices; v++) {
            aiVector3D currVert = currMesh->mVertices[v];
            verts.push_back(glm::vec3(currVert.x, currVert.y, currVert.z) *
                                scale +
                            meshPos);

            aiVector3D currNorm = currMesh->mNormals[v];
            normals.push_back(glm::vec3(currNorm.x, currNorm.y, currNorm.z));
        }

        // get indices
        for (unsigned f = 0; f < currMesh->mNumFaces; f++) {
            aiFace currFace = currMesh->mFaces[f];
            for (unsigned i = 0; i < currFace.mNumIndices; i++) {
                indices.push_back(currFace.mIndices[i]);
            }
        }

        for (unsigned i = 0; i < indices.size(); i += 3) {
            glm::vec3 triPoints[3] = {verts.at(indices.at(i)),
                                      verts.at(indices.at(i + 1)),
                                      verts.at(indices.at(i + 2))};

            glm::vec3 faceNorm = normals.at(indices.at(i)) +
                                 normals.at(indices.at(i + 1)) +
                                 normals.at(indices.at(i + 2));
            faceNorm /= 3.0f;
            faceNorm = glm::normalize(faceNorm);

            tris.push_back(
                std::make_shared<Triangle>(triPoints, objMat, faceNorm));
        }
    }
}

Mesh::~Mesh() {}

bool Mesh::testIntersection(HitData &data) {
    HitData retData(data.getRayOrig(), data.getRayDir());
    retData.setObjDistSq(INFINITY);

    bool triFound = false;

    for (auto t : tris) {
        HitData tempData(data.getRayOrig(), data.getRayDir());
        if (t->testIntersection(tempData)) {
            triFound = true;
            if (tempData.getObjDistSq() < retData.getObjDistSq()) {
                retData = tempData;
            }
        }
    }

    if (triFound) {
        data = retData;
    }
    return triFound;
}