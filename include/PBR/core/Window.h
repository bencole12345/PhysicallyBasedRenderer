#ifndef PHYSICALLYBASEDRENDERER_WINDOW
#define PHYSICALLYBASEDRENDERER_WINDOW

#include <memory>
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

/**
 * An OpenGL window that can be used with a `Renderer` and `Scene` object
 * to render graphics to the user.
 */
class Window {
private:
    GLFWwindow *window;

public:
    Window(const std::string& title, int width, int height);
    ~Window();

    /**
     * Runs the application's main loop.
     */
    void loopUntilClosed(std::shared_ptr<Renderer> renderer, std::shared_ptr<Scene> scene);

private:
    class GLFWCallbackWrapper {
    private:
        static Window* s_window;
        static RendererDriver* s_rendererDriver;

    public:
        GLFWCallbackWrapper() = delete;
        GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
        GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
        ~GLFWCallbackWrapper() = delete;

        static bool isCurrentlyUnbound();

        static void bindWindow(Window* window);
        static void unbindWindow();
        static void bindRendererDriver(RendererDriver* driver);
        static void unbindRendererDriver();

        static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
        static void errorCallback(int error, const char* description);
    };
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_WINDOW
