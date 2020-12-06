#include <iostream>
#include <functional>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/vec3.hpp>

#include "RendererWindow.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "../phong/PhongSceneObject.h"
#include "Camera.h"

namespace PBR {

using namespace std::placeholders;

RendererWindow::RendererWindow(const std::string& title, int width, int height)
        :windowTitle(title), windowWidth(width), windowHeight(height), camera(glm::vec3(0.0, 0.0, 5.0)), scene()
{
    // Set up the callback wrapper
    GLFWCallbackWrapper::SetWindow(this);

    // Set callback for errors
    glfwSetErrorCallback(GLFWCallbackWrapper::ErrorCallback);

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

    // Cull faces that are oriented the wrong way
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_FACE);
    glFrontFace(GL_CCW);
}

void RendererWindow::loadScene(const std::shared_ptr<Scene>& scene)
{
    this->scene = scene;
    scene->activate();
}

void RendererWindow::loop()
{
    if (!scene) {
        throw NoSceneLoadedException();
    }

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

        // Render the scene
        scene->render(camera, currentTime);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Move the camera
        camera.advance((float) dt);

        previousTime = currentTime;
    }
}

RendererWindow::~RendererWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void RendererWindow::keyCallback(int key, int scancode, int action, int mods)
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

    default: break;

    }
}

void RendererWindow::mouseCallback(double positionX, double positionY)
{
    // TODO: Implement
}

void RendererWindow::frameBufferResizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RendererWindow::errorCallback(int error, const char* description)
{
    std::cerr << "ERROR (code = " << error << "): " << description << std::endl;
}

/**
 * Required to avoid a linker error
 */
RendererWindow* RendererWindow::GLFWCallbackWrapper::s_window = nullptr;

void RendererWindow::GLFWCallbackWrapper::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    s_window->keyCallback(key, scancode, action, mods);
}

void RendererWindow::GLFWCallbackWrapper::MousePositionCallback(GLFWwindow* window, double positionX, double positionY)
{
    s_window->mouseCallback(positionX, positionY);
}

void RendererWindow::GLFWCallbackWrapper::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    s_window->frameBufferResizeCallback(window, width, height);
}

void RendererWindow::GLFWCallbackWrapper::SetWindow(RendererWindow* window)
{
    RendererWindow::GLFWCallbackWrapper::s_window = window;
}

void RendererWindow::GLFWCallbackWrapper::ErrorCallback(int error, const char* description)
{
    s_window->errorCallback(error, description);
}

} // namespace PBR
