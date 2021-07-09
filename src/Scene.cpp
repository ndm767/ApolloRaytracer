#include "Scene.hpp"

#include "camera/PerspCamera.hpp"

Scene::Scene(int screenWidth, int screenHeight, glm::vec3 ambientCol,
             float ambientStr) {
    activeCamera = 0;
    cams.push_back(std::make_shared<PerspCamera>(screenWidth, screenHeight,
                                                 glm::vec3(0, 0, 0),
                                                 glm::vec3(0, 0, 1), 60.0f));

    ambientStrength = ambientStr;
    ambientColor = ambientCol;

    useIBL = false;
}

Scene::~Scene() {
    if (useIBL) {
        delete iblImage;
    }
}

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

void Scene::setIBL(std::string path) {
    useIBL = true;
    iblImage = new Texture(path);
}

glm::vec3 Scene::getIBLColor(glm::vec3 dir) {
    // get uv coordinates on image
    constexpr float pi = 3.14159f;
    float u = 0.5f + atan2(dir.x, dir.z) / (2 * pi);
    float v = 0.5f - asin(dir.y) / pi;

    return iblImage->getColorAtPos(u, v);
}