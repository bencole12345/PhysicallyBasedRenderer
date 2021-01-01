# PhysicallyBasedRenderer

A physically-based renderer I'm working on, using C++ and OpenGL. Check back soon for updates!

![Screenshot](screenshot.png "Early screenshot of a textured and an untextured cube rendered with a skybox using the Phong model")

![Screenshot 2](screenshot2.png "Screenshot showing object loading in action")

(Currently I've only implemented Phong shading)

## CMake targets
- The library `pbr`
- An example application `main` that links against `pbr`

## Dependencies (vcpkg)

- `boost-functional`
- `glfw3`
- `glm`
- `opengl`
- `stb`
- `tinyobjloader`
