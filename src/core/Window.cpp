#include "core/Window.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/Renderer.h"
#include "core/RendererDriver.h"

namespace PBR {

Window::Window(const std::string& title, int width, int height)
    : window()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    // Check that the window creation was successful
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create a window." << std::endl;
        exit(1);
    }

    // Register this window with the callback wrapper
    GLFWCallbackWrapper::bindWindow(this);

    // Set callbacks
    glfwSetKeyCallback(window, GLFWCallbackWrapper::keyboardCallback);
    glfwSetFramebufferSizeCallback(window, GLFWCallbackWrapper::frameBufferResizeCallback);
    glfwSetErrorCallback(GLFWCallbackWrapper::errorCallback);

    // Focus the newly created window
    glfwMakeContextCurrent(window);

    // Helps to avoid screen tearing
    glfwSwapInterval(1);
}

Window::~Window()
{
    GLFWCallbackWrapper::unbindWindow();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::loopUntilClosed(std::shared_ptr<Renderer> renderer)
{
    // Compute aspect ratio
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspectRatio = (float)width / (float)height;

    RendererDriver driver(std::move(renderer), aspectRatio);
    GLFWCallbackWrapper::bindRendererDriver(&driver);

    double previousTime = glfwGetTime();

    // TODO: Cap this to target FPS
    while (!glfwWindowShouldClose(window)) {

        // Compute dt
        double currentTime = glfwGetTime();
        float dt = (float) (currentTime - previousTime);

        // Poll events and trigger callbacks
        glfwPollEvents();

        // Update the renderer driver's state
        driver.update(dt);

        // Render the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        driver.render((float) currentTime);

        // Swap the buffers to make the render visible
        glfwSwapBuffers(window);

        previousTime = currentTime;
    }

    GLFWCallbackWrapper::unbindRendererDriver();
}

// These must be present to avoid a linker error
Window* Window::GLFWCallbackWrapper::s_window = nullptr;
RendererDriver* Window::GLFWCallbackWrapper::s_rendererDriver = nullptr;

void Window::GLFWCallbackWrapper::bindWindow(Window* window)
{
    Window::GLFWCallbackWrapper::s_window = window;
}

void Window::GLFWCallbackWrapper::unbindWindow()
{
    Window::GLFWCallbackWrapper::s_window = nullptr;
}

void Window::GLFWCallbackWrapper::bindRendererDriver(RendererDriver* driver)
{
    Window::GLFWCallbackWrapper::s_rendererDriver = driver;
}

void Window::GLFWCallbackWrapper::unbindRendererDriver()
{
    Window::GLFWCallbackWrapper::s_rendererDriver = nullptr;
}

void Window::GLFWCallbackWrapper::keyboardCallback(GLFWwindow*, int key, int scancode, int action, int mods)
{
    assert(s_window);
    assert(s_rendererDriver);

    // Close the window if they pressed escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(s_window->window, GLFW_TRUE);
    }

    // Forward the event to the renderer driver
    s_rendererDriver->onKeyboardEvent(key, scancode, action, mods);
}

void Window::GLFWCallbackWrapper::frameBufferResizeCallback(GLFWwindow*, int width, int height)
{
    assert(s_rendererDriver);

    glViewport(0, 0, width, height);

    float aspectRatio = (float)width / (float)height;
    s_rendererDriver->setAspectRatio(aspectRatio);
}

void Window::GLFWCallbackWrapper::errorCallback(int error, const char* description)
{
    std::cout << "GLFW Error: " << description << std::endl;
    glfwTerminate();
    exit(1);
}

} // namespace PBR
