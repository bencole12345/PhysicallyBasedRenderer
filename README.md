# PhysicallyBasedRenderer

A physically-based renderer I'm working on, using C++ and OpenGL. The renderer has two modes of operation: Phong rendering and physically based rendering, which attempts to model materials and light using physically-accurate properties.

## Screenshots
![Physically Rendered Spheres](screenshots/physically_shaded_spheres_desert.png "Spheres rendered using PBR materials")

From left-to-right the spheres become more metallic, and from bottom-to-top they become rougher.

## CMake targets
- The core library `PBR`.
- Two example applications demonstrating the library's Phong rendering mode, `CubesWithSkybox` and `ObjectLoading`.
- A physically-based example, `PhysicallyRenderedSpheres`.

All examples privately link against the core library. The library includes functions for creating a window, setting up a scene, managing the camera and running the application's main loop.

## Build Dependencies (vcpkg)

- `boost-functional`
- `glew`
- `glfw3`
- `glm`
- `opengl` (this might be preinstalled for you)
- `stb`
- `tinyobjloader`
