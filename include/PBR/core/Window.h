#ifndef PHYSICALLYBASEDRENDERER_WINDOW
#define PHYSICALLYBASEDRENDERER_WINDOW

#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/Renderer.h"
#include "core/RendererDriver.h"
#include "core/Scene.h"

namespace PBR {

class GLFWCallbackWrapper;

/**
 * An OpenGL window that can be used with a `Renderer` and `Scene` object
 * to render graphics to the user.
 */
class Window {
private:
    GLFWwindow* window;

public:
    Window(const std::string& title, int width, int height);
    ~Window();

    /**
     * Runs the application's main loop.
     */
    template<class SceneType>
    void loopUntilClosed(std::shared_ptr<Renderer<SceneType>> renderer, std::shared_ptr<SceneType> scene);
};

/**
 * Forwards GLFW callbacks to the current `Window` instance.
 *
 * This class exists because GLFW is a C library and hence can't call instance
 * methods in callbacks. This class is a bit of a hack that binds a static
 * pointer to the current `Window` instance and forwards the callbacks on.
 */
class GLFWCallbackWrapper {
public:
    typedef std::function<void(GLFWwindow*, int, int, int, int)> KeyboardCallback;
    typedef std::function<void(GLFWwindow*, int, int)> FrameBufferResizeCallback;
    typedef std::function<void(int, const char*)> ErrorCallback;

private:
    static std::optional<KeyboardCallback> s_keyboardCallback;
    static std::optional<FrameBufferResizeCallback> s_frameBufferResizeCallback;
    static std::optional<ErrorCallback> s_errorCallback;

public:
    GLFWCallbackWrapper() = delete;
    GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
    GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
    ~GLFWCallbackWrapper() = delete;

    static void bindKeyboardCallback(const KeyboardCallback& keyboardCallback);
    static void bindFrameBufferResizeCallback(const FrameBufferResizeCallback& frameBufferResizeCallback);
    static void bindErrorCallback(const ErrorCallback& errorCallback);

    static void unbindAllCallbacks();

    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
    static void errorCallback(int error, const char* description);
};

template <class SceneType>
void Window::loopUntilClosed(std::shared_ptr<Renderer<SceneType>> renderer, std::shared_ptr<SceneType> scene)
{
    // Compute aspect ratio
    float aspectRatio;
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        aspectRatio = (float) width / (float) height;
    }

    // TODO: Check that it's guaranteed that all scenes have a background colour
    glm::vec3 backgroundColour = scene->getBackgroundColour();
    glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 1.0f);

    // Create a driver to handle camera movement
    RendererDriver driver(std::move(renderer), aspectRatio, std::move(scene));

    // Callback for keyboard events
    GLFWCallbackWrapper::bindKeyboardCallback(
        [&driver](GLFWwindow *theWindow, int key, int scancode, int action, int mods) {

            // If they pressed escape then we should close
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(theWindow, GLFW_TRUE);
            }

            // Forward it on to the renderer driver
            driver.onKeyboardEvent(key, scancode, action, mods);
        }
    );

    // Callback for frame buffer resize events
    GLFWCallbackWrapper::bindFrameBufferResizeCallback(
        [&driver](GLFWwindow*, int width, int height) {
            glViewport(0, 0, width, height);
            float ratio = (float) width / (float) height;
            driver.setAspectRatio(ratio);
        }
    );

    // Callback for errors
    GLFWCallbackWrapper::bindErrorCallback(
        [](int error, const char *description) {
            std::cout << "GLFW Error: " << description << std::endl;
            glfwTerminate();
            exit(1);
        }
    );

    double previousTime = glfwGetTime();

    // The main loop
    while (!glfwWindowShouldClose(window)) {

        // Compute dt
        double currentTime = glfwGetTime();
        auto dt = (float) (currentTime - previousTime);

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

    GLFWCallbackWrapper::unbindAllCallbacks();
}


} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_WINDOW
