#include <iostream>
#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/RendererWindow.h"
#include "phong/BasicPhongScene.h"
#include "example/ExamplePhongSceneBuilder.h"

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
    PBR::RendererWindow window(title, width, height);

    // Create a scene
    std::shared_ptr<PBR::BasicPhongScene> scene;
    PBR::ExamplePhongSceneBuilder::loadBasicPhongScene(&scene);

    // Load the scene
    window.loadScene(scene);

    // Run the main loop
    window.loop();

    return 0;
}
