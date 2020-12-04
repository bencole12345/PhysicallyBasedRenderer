#ifndef PHYSICALLYBASEDRENDERER_WINDOW
#define PHYSICALLYBASEDRENDERER_WINDOW

#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include "Camera.h"

namespace PBR {

class FailedToCreateWindowException : public std::exception { };

class Window {
private:
    const std::string& windowTitle;
    int windowWidth;
    int windowHeight;
    GLFWwindow* window;

    Camera camera;

public:
    Window(const std::string& title, int width, int height);
    ~Window();

    /**
     * Run the main application loop.
     */
    void loop();

private:
    void keyCallback(int key, int scancode, int action, int mods);
    void mouseCallback(double positionX, double positionY);
    void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
    void errorCallback(int error, const char* description);

    /**
     * This exists because you can't bind references to function pointers for GLFW
     * callbacks because it's a C library. This wrapper class just exposes static
     * methods and forwards on the arguments to the actual callback.
     */
    class GLFWCallbackWrapper {

    private:

        /**
         * Pointer to the containing window used for
         */
        static Window* s_window;

    public:

        // The class contains only static attributes and methods so you shouldn't be
        // able to instantiate it.
        GLFWCallbackWrapper() = delete;
        GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
        GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
        ~GLFWCallbackWrapper() = delete;

        static void SetWindow(Window* window);

        static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MousePositionCallback(GLFWwindow* window, double positionX, double positionY);
        static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
        static void ErrorCallback(int error, const char* description);
    };
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_WINDOW
