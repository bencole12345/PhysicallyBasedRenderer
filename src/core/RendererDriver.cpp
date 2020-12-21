#include "core/RendererDriver.h"

#define DEFAULT_MOVE_SPEED 1.0
#define DEFAULT_TURN_SPEED 0.7

#include <memory>
#include <utility>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/vec3.hpp>

namespace PBR {

MovementState::MovementState()
        :movingForwards(false), movingBackwards(false), movingLeft(false), movingRight(false), movingUp(false),
         movingDown(false), rotatingUp(false), rotatingDown(false), rotatingLeft(false), rotatingRight(false)
{
}

RendererDriver::RendererDriver(std::shared_ptr<Renderer> renderer, float aspectRatio)
        :camera(glm::vec3(0.0, 0.0, 5.0), aspectRatio), movementState(), moveSpeed(DEFAULT_MOVE_SPEED),
         turnSpeed(DEFAULT_TURN_SPEED), renderer(std::move(renderer))
{
}

void RendererDriver::setAspectRatio(float aspectRatio)
{
    camera.setAspectRatio(aspectRatio);
}

void RendererDriver::update(float dt)
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

void RendererDriver::render(float time)
{
    renderer->render(camera, time);
}

void RendererDriver::onKeyboardEvent(int key, int scancode, int action, int mods)
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
