# ApolloRaytracer

A hobby blinn-phong shaded ray-tracer written in C++

## Features

- SDL2 Window output
- Keyboard controls
- Blinn-Phong shading
- Reflections
- Refractions
- Basic multithreading
- Sphere geometry
- Triangle geometry
- Mesh geometry / model loading
- AABB for mesh-ray collisions
- Environment mapping
- Basic material system
- ASCII output (doesn't play well with environment mapping, works better if you decrease terminal font, can be enabled in `ApolloRaytracer.cpp`)
- JSON scene loading
- Multiple cameras and ability to cycle through them
- Multiple lights

## Controls

- WASD to move  
- QE to move up and down
- arrow keys for camera rotation
- space bar to cycle through cameras in scene
- scenes are loaded using a JSON file (examples can be seen in `assets/scenes/`)
- command line options can be seen by running `./apollo --help`

## Building

`make [-j#]`  
`./apollo`

## Dependencies

- SDL2
- Assimp
- glm
- nlohmann/json
- jarro2783/cxxopts
