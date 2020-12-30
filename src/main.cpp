#include <iostream>
#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/Renderer.h"
#include "core/Scene.h"
#include "core/Window.h"
#include "example/ExampleSceneBuilder.h"
#include "phong/PhongRenderer.h"

using namespace PBR;

int main()
{
    std::string title = "Physically Based Renderer";
    int width = 800;
    int height = 600;

    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW." << std::endl;
        return 1;
    }

    // Create a window
    Window window(title, width, height);

    // Create a scene
    std::shared_ptr<Scene> scene;
    ExampleSceneBuilder::loadBasicScene(&scene);

    // Create a Phong renderer
    std::shared_ptr<Renderer> renderer(new phong::PhongRenderer());

    // Run the main loop
    window.loopUntilClosed(renderer, scene);

    return 0;
}
