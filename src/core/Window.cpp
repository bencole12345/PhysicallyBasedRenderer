#include "core/Window.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <glm/vec3.hpp>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/Renderer.h"
#include "core/RendererDriver.h"

namespace PBR {

Window::Window(const std::string& title, int width, int height)
        :window()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW." << std::endl;
        exit(1);
    }

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
    glfwDestroyWindow(window);
    glfwTerminate();
}

// These must be present to avoid a linker error
std::optional<GLFWCallbackWrapper::KeyboardCallback> GLFWCallbackWrapper::s_keyboardCallback;
std::optional<GLFWCallbackWrapper::FrameBufferResizeCallback> GLFWCallbackWrapper::s_frameBufferResizeCallback;
std::optional<GLFWCallbackWrapper::ErrorCallback> GLFWCallbackWrapper::s_errorCallback;

void GLFWCallbackWrapper::bindKeyboardCallback(const KeyboardCallback& keyboardCallback)
{
    assert(!s_keyboardCallback.has_value() && "Cannot bind multiple callbacks at once.");
    s_keyboardCallback = keyboardCallback;
}

void GLFWCallbackWrapper::bindFrameBufferResizeCallback(const FrameBufferResizeCallback& frameBufferResizeCallback)
{
    assert(!s_frameBufferResizeCallback.has_value() && "Cannot bind multiple callbacks at once.");
    s_frameBufferResizeCallback = frameBufferResizeCallback;
}

void GLFWCallbackWrapper::bindErrorCallback(const ErrorCallback& errorCallback)
{
    assert(!s_errorCallback.has_value() && "Cannot bind multiple callbacks at once.");
    s_errorCallback = s_errorCallback;
}

void GLFWCallbackWrapper::unbindAllCallbacks()
{
    s_keyboardCallback.reset();
    s_frameBufferResizeCallback.reset();
    s_errorCallback.reset();
}

void GLFWCallbackWrapper::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (s_keyboardCallback.has_value()) {
        auto& callback = s_keyboardCallback.value();
        callback(window, key, scancode, action, mods);
    }
}

void GLFWCallbackWrapper::frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    if (s_frameBufferResizeCallback.has_value()) {
        auto& callback = s_frameBufferResizeCallback.value();
        callback(window, width, height);
    }
}

void GLFWCallbackWrapper::errorCallback(int error, const char* description)
{
    if (s_errorCallback.has_value()) {
        auto& callback = s_errorCallback.value();
        callback(error, description);
    }
}

} // namespace PBR
