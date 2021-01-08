#ifndef PHYSICALLYBASEDRENDERER_RENDERERDRIVER
#define PHYSICALLYBASEDRENDERER_RENDERERDRIVER

#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "core/Renderer.h"
#include "core/Scene.h"

#define DEFAULT_MOVE_SPEED 1.0
#define DEFAULT_TURN_SPEED 0.7

namespace PBR {

/**
 * Wraps all flags for whether we are currently moving and rotating.
 */
struct MovementState {

    // Movement
    bool movingForwards{false};
    bool movingBackwards{false};
    bool movingLeft{false};
    bool movingRight{false};
    bool movingUp{false};
    bool movingDown{false};

    // Rotation
    bool rotatingUp{false};
    bool rotatingDown{false};
    bool rotatingLeft{false};
    bool rotatingRight{false};
};

/**
 * Manages moving a Camera according to user inputs and uses this
 * to call the render() method of a Renderer object.
 */
template<class SceneType>
class RendererDriver {
private:
    Camera camera;
    MovementState movementState;
    float moveSpeed;
    float turnSpeed;
    std::shared_ptr<Renderer<SceneType>> renderer;
    std::shared_ptr<SceneType> scene;

public:
    RendererDriver(std::shared_ptr<Renderer<SceneType>> renderer, float aspectRatio, std::shared_ptr<SceneType> scene);

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

template<class SceneType>
RendererDriver<SceneType>::RendererDriver(std::shared_ptr<Renderer<SceneType>> renderer, float aspectRatio,
                                          std::shared_ptr<SceneType> scene)
        :camera(glm::vec3(0.0, 0.0, 5.0), aspectRatio), movementState(), moveSpeed(DEFAULT_MOVE_SPEED),
         turnSpeed(DEFAULT_TURN_SPEED), renderer(std::move(renderer)), scene(std::move(scene))
{
    this->renderer->activate();
}

template<class SceneType>
void RendererDriver<SceneType>::setAspectRatio(float aspectRatio)
{
    camera.setAspectRatio(aspectRatio);
}

template<class SceneType>
void RendererDriver<SceneType>::update(float dt)
{
    float movementDistance = moveSpeed * dt;
    float rotationAngle = turnSpeed * dt;

    if (movementState.movingForwards && !movementState.movingBackwards)
        camera.moveForwards(movementDistance);

    if (movementState.movingBackwards && !movementState.movingForwards)
        camera.moveBackwards(movementDistance);

    if (movementState.movingLeft && !movementState.movingRight)
        camera.moveLeft(movementDistance);

    if (movementState.movingRight && !movementState.movingLeft)
        camera.moveRight(movementDistance);

    if (movementState.movingUp && !movementState.movingDown)
        camera.moveUp(movementDistance);

    if (movementState.movingDown && !movementState.movingUp)
        camera.moveDown(movementDistance);

    if (movementState.rotatingLeft && !movementState.rotatingRight)
        camera.rotateLeft(rotationAngle);

    if (movementState.rotatingRight && !movementState.rotatingLeft)
        camera.rotateRight(rotationAngle);

    if (movementState.rotatingUp && !movementState.rotatingDown)
        camera.rotateUp(rotationAngle);

    if (movementState.rotatingDown && !movementState.rotatingUp)
        camera.rotateDown(rotationAngle);
}

template<class SceneType>
void RendererDriver<SceneType>::render(float time)
{
    renderer->render(scene, camera, time);
}

template<class SceneType>
void RendererDriver<SceneType>::onKeyboardEvent(int key, int scancode, int action, int mods)
{
    switch (key) {

    case GLFW_KEY_W:
        if (action == GLFW_PRESS)
            movementState.movingForwards = true;
        else if (action == GLFW_RELEASE)
            movementState.movingForwards = false;
        break;

    case GLFW_KEY_A:
        if (action == GLFW_PRESS)
            movementState.movingLeft = true;
        else if (action == GLFW_RELEASE)
            movementState.movingLeft = false;
        break;

    case GLFW_KEY_S:
        if (action == GLFW_PRESS)
            movementState.movingBackwards = true;
        else if (action == GLFW_RELEASE)
            movementState.movingBackwards = false;
        break;

    case GLFW_KEY_D:
        if (action == GLFW_PRESS)
            movementState.movingRight = true;
        else if (action == GLFW_RELEASE)
            movementState.movingRight = false;
        break;

    case GLFW_KEY_R:
        if (action == GLFW_PRESS)
            movementState.movingUp = true;
        else if (action == GLFW_RELEASE)
            movementState.movingUp = false;
        break;

    case GLFW_KEY_F:
        if (action == GLFW_PRESS)
            movementState.movingDown = true;
        else if (action == GLFW_RELEASE)
            movementState.movingDown = false;
        break;

    case GLFW_KEY_UP:
        if (action == GLFW_PRESS)
            movementState.rotatingUp = true;
        else if (action == GLFW_RELEASE)
            movementState.rotatingUp = false;
        break;

    case GLFW_KEY_DOWN:
        if (action == GLFW_PRESS)
            movementState.rotatingDown = true;
        else if (action == GLFW_RELEASE)
            movementState.rotatingDown = false;
        break;

    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS)
            movementState.rotatingLeft = true;
        else if (action == GLFW_RELEASE)
            movementState.rotatingLeft = false;
        break;

    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS)
            movementState.rotatingRight = true;
        else if (action == GLFW_RELEASE)
            movementState.rotatingRight = false;
        break;

    default: break;

    }
}

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_RENDERERDRIVER
