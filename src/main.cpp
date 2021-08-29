#include "ApolloRaytracer.hpp"

#include <cxxopts.hpp>

int main(int argc, char *argv[]) {

    cxxopts::Options options("Apollo Raytracer",
                             "A hobby raytracer written in C++");

    // clang-format off

    options.add_options()
    ("s,scene", "specify scene file to load", cxxopts::value<std::string>()->default_value("assets/scenes/testScene.json"))
    ("w,width", "specify output width", cxxopts::value<int>()->default_value("400"))
    ("h,height", "specify output height", cxxopts::value<int>()->default_value("400"))
    ("r,res", "specify number of pixels per ray", cxxopts::value<int>()->default_value("1"))
    ("d,depth", "specify maximum ray depth", cxxopts::value<int>()->default_value("3"))
    ("n,numsamples", "number of samples per pixel", cxxopts::value<int>()->default_value("1"))
    ("help", "prints help message");   
    ;

    // clang-format on

    auto results = options.parse(argc, argv);

    if (results.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::string scene = results["scene"].as<std::string>();
    int width = results["width"].as<int>();
    int height = results["height"].as<int>();
    int res = results["res"].as<int>();
    int depth = results["depth"].as<int>();
    int samples = results["numsamples"].as<int>();

    ApolloRaytracer apollo(scene, width, height, res, depth, samples);
    apollo.run();
    return 0;
}