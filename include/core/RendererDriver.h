#ifndef PHYSICALLYBASEDRENDERER_RENDERERDRIVER
#define PHYSICALLYBASEDRENDERER_RENDERERDRIVER

#include <memory>

#include "Camera.h"
#include "core/Renderer.h"

namespace PBR {

/**
 * Wraps all flags for whether we are currently moving and rotating.
 */
struct MovementState {

    // Movement
    int movingForwards : 1;
    int movingBackwards : 1;
    int movingLeft : 1;
    int movingRight : 1;
    int movingUp : 1;
    int movingDown : 1;

    // Rotation
    int rotatingUp : 1;
    int rotatingDown : 1;
    int rotatingLeft : 1;
    int rotatingRight : 1;

    // Default constructor to initialise everything to zero
    MovementState();
};

/**
 * Manages moving a Camera according to user inputs and uses this
 * to call the render() method of a Renderer object.
 */
class RendererDriver {
private:
    Camera camera;
    MovementState movementState;
    float moveSpeed;
    float turnSpeed;
    std::shared_ptr<Renderer> renderer;

public:
    RendererDriver(std::shared_ptr<Renderer> renderer, float aspectRatio);

    void setAspectRatio(float aspectRatio);

    /**
     * Update the state for a new frame.
     */
    void update(float dt);

    /**
     * Render to the window using the driven renderer.
     */
    void render(float time);

    /**
     * Respond to a keyboard event.
     */
    void onKeyboardEvent(int key, int scancode, int action, int mods);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_RENDERERDRIVER
