# PhysicallyBasedRenderer

A physically-based renderer I'm working on, using C++ and OpenGL. The renderer has two modes of operation: Phong rendering and physically based rendering, which attempts to model materials and light using physically-accurate properties.

The project is still under development, so check back soon for updates!

## Screenshots [Physically Based Mode]
![Physically Rendered Spheres](screenshots/physically_shaded_spheres_ibl_fake_specular.png "Spheres rendered using PBR materials")

[Screenshot taken while I'm still working on proper image based specular lighting: for now it's a bit of a hack!]

From left-to-right the spheres become more metallic, and from bottom-to-top they become rougher.

## Screenshots [Phong Mode]
![Textured Cubes and Skybox](screenshots/phong_textured_cubes_with_skybox.png "Screenshot of a textured and an untextured cube rendered with a skybox using the Phong model")
![Object Loading](screenshots/phong_object_loading.png "Screenshot showing object loading in action using the Phong model")

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
