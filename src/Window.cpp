#include <iostream>
#include <string>
#include <functional>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "Window.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
#include "Camera.h"

namespace PBR {

using namespace std::placeholders;

Window::Window(const std::string& title, int width, int height)
        :windowTitle(title), windowWidth(width), windowHeight(height), camera()
{
    // Set up the callback wrapper
    GLFWCallbackWrapper::SetWindow(this);

    // Set callback for errors
    glfwSetErrorCallback(GLFWCallbackWrapper::ErrorCallback);

    // Initialise the library
    if (!glfwInit()) {
        throw FailedToCreateWindowException();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw FailedToCreateWindowException();
    }

    // Set callbacks
    glfwSetKeyCallback(window, GLFWCallbackWrapper::KeyboardCallback);
    glfwSetFramebufferSizeCallback(window, GLFWCallbackWrapper::FrameBufferResizeCallback);

    // Focus current window
    glfwMakeContextCurrent(window);

    // Used to avoid screen tearing
    glfwSwapInterval(1);

    // Turn on the z buffer
    glEnable(GL_DEPTH_TEST);

    // TODO: Turn on the setting to cull faces oriented the wrong way
}

void Window::loop()
{
    // OpenGL initializations start from here
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Vertex data and buffer
    float vertices[] = {
            -0.5f, -0.5f, -5.0f,
            0.5f, -0.5f, -5.0f,
            0.0f, 0.5f, -5.0f
    };

    // Set up a basic triangle scene object
    Shader vertexShader("shaders/vertex/test.vert", GL_VERTEX_SHADER);
    Shader fragmentShader("shaders/fragment/test.frag", GL_FRAGMENT_SHADER);
    ShaderProgram program(vertexShader, fragmentShader);
    SceneObject object(vertices, sizeof(vertices), program);

    double previousTime = glfwGetTime();

    // The main loop
    while (!glfwWindowShouldClose(window)) {

        // Compute dt
        double currentTime = glfwGetTime();
        double dt = currentTime - previousTime;

        // Resize the viewport
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // OpenGL Rendering related code
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the triangle
        object.render(camera.getMVPMatrix());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Move the camera
        camera.advance((float) dt);

        previousTime = currentTime;
    }
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::keyCallback(int key, int scancode, int action, int mods)
{
    switch (key) {

    case GLFW_KEY_ESCAPE: {
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    } break;

    case GLFW_KEY_W: {
        if (action == GLFW_PRESS) {
            camera.setMovingForwards(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setMovingForwards(false);
        }
    } break;

    case GLFW_KEY_A: {
        if (action == GLFW_PRESS) {
            camera.setMovingLeft(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setMovingLeft(false);
        }
    } break;

    case GLFW_KEY_S: {
        if (action == GLFW_PRESS) {
            camera.setMovingBackwards(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setMovingBackwards(false);
        }
    } break;

    case GLFW_KEY_D: {
        if (action == GLFW_PRESS) {
            camera.setMovingRight(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setMovingRight(false);
        }
    } break;

    case GLFW_KEY_R: {
        if (action == GLFW_PRESS) {
            camera.setMovingUp(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setMovingUp(false);
        }
    } break;

    case GLFW_KEY_F: {
        if (action == GLFW_PRESS) {
            camera.setMovingDown(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setMovingDown(false);
        }
    } break;

    case GLFW_KEY_UP: {
        if (action == GLFW_PRESS) {
            camera.setRotatingUp(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setRotatingUp(false);
        }
    } break;

    case GLFW_KEY_DOWN: {
        if (action == GLFW_PRESS) {
            camera.setRotatingDown(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setRotatingDown(false);
        }
    } break;

    case GLFW_KEY_LEFT: {
        if (action == GLFW_PRESS) {
            camera.setRotatingLeft(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setRotatingLeft(false);
        }
    } break;

    case GLFW_KEY_RIGHT: {
        if (action == GLFW_PRESS) {
            camera.setRotatingRight(true);
        }
        else if (action == GLFW_RELEASE) {
            camera.setRotatingRight(false);
        }
    } break;

    }
}

void Window::mouseCallback(double positionX, double positionY)
{
    // TODO: Implement
}

void Window::frameBufferResizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::errorCallback(int error, const char* description)
{
    std::cerr << "ERROR (code = " << error << "): " << description << std::endl;
}

/**
 * Required to avoid a linker error
 */
Window* Window::GLFWCallbackWrapper::s_window = nullptr;

void Window::GLFWCallbackWrapper::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    s_window->keyCallback(key, scancode, action, mods);
}

void Window::GLFWCallbackWrapper::MousePositionCallback(GLFWwindow* window, double positionX, double positionY)
{
    s_window->mouseCallback(positionX, positionY);
}

void Window::GLFWCallbackWrapper::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    s_window->frameBufferResizeCallback(window, width, height);
}

void Window::GLFWCallbackWrapper::SetWindow(Window* window)
{
    Window::GLFWCallbackWrapper::s_window = window;
}

void Window::GLFWCallbackWrapper::ErrorCallback(int error, const char* description)
{
    s_window->errorCallback(error, description);
}

} // namespace PBR
