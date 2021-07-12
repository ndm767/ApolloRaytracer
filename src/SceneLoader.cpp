#include "SceneLoader.hpp"
#include "FileLoader.hpp"
#include "Material.hpp"
#include "camera/PerspCamera.hpp"
#include "geometry/Mesh.hpp"
#include "geometry/Sphere.hpp"
#include "light/Light.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

SceneLoader::SceneLoader() {}

SceneLoader::~SceneLoader() {}

glm::vec3 SceneLoader::jArrToVec(jArr arr) {
    glm::vec3 ret = glm::vec3(0, 0, 0);
    ret.x = arr.at(0).get<float>();
    ret.y = arr.at(1).get<float>();
    ret.z = arr.at(2).get<float>();
    return ret;
}

void SceneLoader::loadScene(int screenWidth, int screenHeight, int rayDepth,
                            std::string path, Scene &scene) {

    std::ifstream f(path);
    if (!f) {
        std::cout << "Could not open file " << path << std::endl;
    }

    nlohmann::json parsedScene;
    f >> parsedScene;

    // add cameras
    if (parsedScene["cameras"].size() > 0) {
        for (int i = 0; i < parsedScene["cameras"].size(); i++) {

            auto currCam =
                parsedScene["cameras"][std::to_string(i)].get<nlohmann::json>();

            if (currCam["type"].get<std::string>() == "persp") {
                // position
                jArr camPos = currCam["pos"].get<jArr>();
                glm::vec3 inPos = jArrToVec(camPos);
                // direction
                jArr camDir = currCam["dir"].get<jArr>();
                glm::vec3 inDir = jArrToVec(camDir);
                // fov
                float hFov = currCam["fov"].get<float>();
                scene.addCamera(std::make_shared<PerspCamera>(
                    screenWidth, screenHeight, inPos, inDir, hFov, rayDepth));
            }
        }
    }

    // add materials
    std::vector<Material> mats;

    if (parsedScene["materials"].size() > 0) {
        for (int i = 0; i < parsedScene["materials"].size(); i++) {
            auto currMat = parsedScene["materials"][std::to_string(i)]
                               .get<nlohmann::json>();
            // diffuse
            jArr matDiff = currMat["diffuse"].get<jArr>();
            glm::vec3 diffuse = jArrToVec(matDiff);

            // specular
            jArr matSpec = currMat["specular"].get<jArr>();
            glm::vec3 specular = jArrToVec(matSpec);

            // phong exponent
            float exp = currMat["exp"].get<float>();

            Material mat(diffuse, specular, exp);

            // reflection
            auto reflect = currMat["reflection"].get<nlohmann::json>();
            if (reflect["use"].get<bool>()) {
                mat.setUseReflection(true);
                mat.setReflectionCoef(reflect["coef"].get<float>());
            }

            // refraction
            auto refract = currMat["refraction"].get<nlohmann::json>();
            if (refract["use"].get<bool>()) {
                mat.setUseRefraction(true);
                mat.setIndexOfRefraction(refract["index"].get<float>());
                mat.setRefractionCoef(refract["coef"].get<float>());
            }

            mats.push_back(mat);
        }
    }

    // add objects
    if (parsedScene["objects"].size() > 0) {
        for (int i = 0; i < parsedScene["objects"].size(); i++) {

            auto currObj =
                parsedScene["objects"][std::to_string(i)].get<nlohmann::json>();

            if (currObj["type"].get<std::string>() == "sphere") {
                // material
                int matNum = currObj["material"].get<int>();
                // radius
                float radius = currObj["radius"].get<float>();
                // position
                jArr objPos = currObj["pos"].get<jArr>();
                glm::vec3 inPos = jArrToVec(objPos);

                scene.addObject(
                    std::make_shared<Sphere>(inPos, radius, mats.at(matNum)));
            } else if (currObj["type"].get<std::string>() == "mesh") {
                // material
                int matNum = currObj["material"].get<int>();
                // scale
                float scale = currObj["scale"].get<float>();
                // position
                jArr objPos = currObj["pos"].get<jArr>();
                glm::vec3 pos = jArrToVec(objPos);
                // path
                std::string path = currObj["source"].get<std::string>();

                FileLoader fl;
                fl.loadFile(path, scene, pos, scale, &mats.at(matNum));
            }
        }
    }

    // add lights
    if (parsedScene["lights"].size() > 0) {
        for (int i = 0; i < parsedScene["lights"].size(); i++) {

            auto currLight =
                parsedScene["lights"][std::to_string(i)].get<nlohmann::json>();

            // position
            jArr lightPos = currLight["position"].get<jArr>();
            glm::vec3 pos = jArrToVec(lightPos);
            // intensity
            float intensity = currLight["intensity"].get<float>();
            // color
            jArr lightCol = currLight["color"].get<jArr>();
            glm::vec3 col = jArrToVec(lightCol);

            scene.addLight(std::make_shared<Light>(pos, intensity, col));
        }
    }

    // misc settings
    if (parsedScene["misc"].contains("envMap")) {
        scene.setEnv(parsedScene["misc"]["envMap"].get<std::string>());
    }

    if (parsedScene["misc"].contains("activeCam")) {
        scene.setActiveCamera(parsedScene["misc"]["activeCam"].get<int>());
    }
}