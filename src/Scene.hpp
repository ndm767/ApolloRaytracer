#pragma once

#include "Texture.hpp"
#include "camera/Camera.hpp"
#include "geometry/Object.hpp"
#include "light/Light.hpp"

#include <memory>
#include <vector>

// class that hold entire scene
class Scene {
  public:
    Scene(int width, int height, glm::vec3 ambientCol, float ambientStr);
    ~Scene();

    // add functions return the index of the thing added
    int addObject(std::shared_ptr<Object> o);
    int addCamera(std::shared_ptr<Camera> c);
    int addLight(std::shared_ptr<Light> l);

    std::vector<std::shared_ptr<Object>> &getObjects() { return objs; }
    std::vector<std::shared_ptr<Light>> &getLights() { return lights; }

    int getNumCameras() { return cams.size(); }

    std::shared_ptr<Camera> getActiveCamera() { return cams.at(activeCamera); }
    void setActiveCamera(unsigned index);

    glm::vec3 getAmbientColor() { return ambientColor; }
    float getAmbientStrength() { return ambientStrength; }

    void setIBL(std::string path);

    bool getUseIBL() { return useIBL; }
    glm::vec3 getIBLColor(glm::vec3 dir);

  protected:
    std::vector<std::shared_ptr<Object>> objs;
    std::vector<std::shared_ptr<Camera>> cams;
    std::vector<std::shared_ptr<Light>> lights;

    glm::vec3 ambientColor;
    float ambientStrength;

    unsigned activeCamera;

    // image-based lighting
    bool useIBL;
    Texture *iblImage;
};