#include <iostream>
#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/Renderer.h"
#include "core/Window.h"
#include "example/ExamplePhongSceneBuilder.h"
#include "phong/PhongRenderer.h"
#include "phong/PhongScene.h"

using namespace PBR;

int main()
{
    std::string title = "Physically-Based Renderer";
    int width = 640;
    int height = 480;

    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW." << std::endl;
        return 1;
    }

    // Create a window
    Window window(title, width, height);

    // Create a scene
    std::shared_ptr<phong::PhongScene> scene;
    ExamplePhongSceneBuilder::loadBasicPhongScene(&scene);

    // Create a renderer
    std::shared_ptr<Renderer> renderer(new phong::PhongRenderer(scene));

    // Run the main loop
    window.loopUntilClosed(renderer);

    return 0;
}
