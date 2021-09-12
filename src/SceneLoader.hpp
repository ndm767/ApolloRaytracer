#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include "Scene.hpp"

typedef nlohmann::json::array_t jArr;

class SceneLoader {
  public:
    SceneLoader();
    ~SceneLoader();

    void loadScene(int screenWidth, int screenHeight, int rayDepth,
                   std::string path, Scene &scene, bool useMesh);

  protected:
    glm::vec3 jArrToVec(jArr arr);
};