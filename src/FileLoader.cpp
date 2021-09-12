#include "FileLoader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

FileLoader::FileLoader() {}

FileLoader::~FileLoader() {}

void FileLoader::loadFile(std::string path, Scene &targetScene, glm::vec3 pos,
                          float scale, Material *mat, bool useMesh) {
    Assimp::Importer imp;
    const aiScene *scene = imp.ReadFile(
        path.c_str(),
        aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GenNormals |
            aiProcess_OptimizeMeshes | aiProcess_PreTransformVertices |
            aiProcess_JoinIdenticalVertices | aiProcess_FindDegenerates |
            aiProcess_FindInvalidData);

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
            glm::vec3 glmVert = glm::vec3(currVert.x, currVert.y, currVert.z);
            verts.push_back(glmVert);

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

        MeshInput mi;
        mi.vertices = verts;
        mi.indices = indices;
        mi.normals = normals;

        Material temp(glm::vec3(0.5f), glm::vec3(0.5f), 100);
        temp.setUseReflection(true);
        temp.setReflectionCoef(0.5f);

        Material matToUse = temp;
        if (mat != nullptr) {
            matToUse = *mat;
        }

        if (useMesh) {
            targetScene.addObject(
                std::make_shared<Mesh>(mi, pos, scale, matToUse));
        } else {

            Mesh tempMesh(mi, pos, scale, matToUse);

            // the best performance is gotten when the number of subvoxels
            // roughly equals the number of triangles in the model this is
            // because if there are less subvoxels, then multiple triangles have
            // to be tested per subvoxel and if there are less, then we are
            // traversing excessive subvoxels
            int numTris = tempMesh.getNumTris();
            int numSubVoxels = 1;
            int numLayers = 0;
            while (numSubVoxels < numTris) {
                numSubVoxels *= 8;
                numLayers++;
            }

            targetScene.addObject(
                std::make_shared<OctreeMesh>(tempMesh, numLayers));
        }
        // targetScene.addObject(std::make_shared<Mesh>(mi, pos, scale,
        // matToUse));
    }
}