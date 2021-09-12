#pragma once

#include "Scene.hpp"
#include "geometry/Mesh.hpp"
#include "geometry/OctreeMesh.hpp"

#include <string>

class FileLoader {
  public:
    FileLoader();
    ~FileLoader();

    void loadFile(std::string path, Scene &targetScene, glm::vec3 pos,
                  float scale, Material *mat, bool useMesh);
};