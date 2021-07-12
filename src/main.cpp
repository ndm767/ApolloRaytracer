#include "ApolloRaytracer.hpp"

int main(int argc, char *argv[]) {
    ApolloRaytracer apollo("assets/scenes/testScene.json", 400, 400, 4);
    apollo.run();
    return 0;
}