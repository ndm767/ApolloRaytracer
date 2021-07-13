# ApolloRaytracer

A hobby Blinn-Phong shaded ray-tracer written in C++

![teapots](https://user-images.githubusercontent.com/5031736/125385940-76416300-e369-11eb-9124-a590e2d20940.png)
![testscene](https://user-images.githubusercontent.com/5031736/125385958-7f323480-e369-11eb-9149-007615d59c52.png)
![lucy](https://user-images.githubusercontent.com/5031736/125386003-8eb17d80-e369-11eb-849d-833af8215e0b.png)
![asciiraytracer](https://user-images.githubusercontent.com/5031736/125386017-92dd9b00-e369-11eb-91ae-e02d83bd6196.png)


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

## Attributions

- models gotten from https://github.com/alecjacobson/common-3d-test-models
- models gotten from http://graphics.stanford.edu/data/3Dscanrep/
- hdrs gotten from https://polyhaven.com

