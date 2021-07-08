#pragma once

#include "camera/Camera.hpp"
#include "geometry/Object.hpp"

#include <memory>
#include <vector>

// class that hold entire scene
class Scene {
  public:
    Scene(int width, int height);
    ~Scene();

    // add functions return the index of the thing added
    int addObject(std::shared_ptr<Object> o);
    int addCamera(std::shared_ptr<Camera> c);

    std::vector<std::shared_ptr<Object>> &getObjects() { return objs; }

    std::shared_ptr<Camera> getActiveCamera() { return cams.at(activeCamera); }
    void setActiveCamera(unsigned index);

  protected:
    std::vector<std::shared_ptr<Object>> objs;
    std::vector<std::shared_ptr<Camera>> cams;

    unsigned activeCamera;
};