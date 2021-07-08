#include "Scene.hpp"

#include "camera/PerspCamera.hpp"

Scene::Scene(int screenWidth, int screenHeight) {
    activeCamera = 0;
    cams.push_back(std::make_shared<PerspCamera>(screenWidth, screenHeight,
                                                 glm::vec3(0, 0, 0),
                                                 glm::vec3(0, 0, 1), 60.0f));
}

Scene::~Scene() {}

int Scene::addObject(std::shared_ptr<Object> o) {
    objs.push_back(o);
    return objs.size() - 1;
}

int Scene::addCamera(std::shared_ptr<Camera> c) {
    cams.push_back(c);
    return cams.size() - 1;
}

int Scene::addLight(std::shared_ptr<Light> l) {
    lights.push_back(l);
    return lights.size() - 1;
}

void Scene::setActiveCamera(unsigned index) {
    if (index < cams.size()) {
        activeCamera = index;
    }
}