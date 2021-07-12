#include "ApolloRaytracer.hpp"

int main(int argc, char *argv[]) {
    ApolloRaytracer apollo("assets/scenes/teapots.json", 400, 400, 1, 3);
    apollo.run();
    return 0;
}