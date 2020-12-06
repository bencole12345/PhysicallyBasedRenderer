#include "Camera.h"

#define MOVE_SPEED 1.0
#define ROTATION_SPEED 0.7

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace PBR {

MovingState::MovingState()
        :movingForwards(false), movingBackwards(false), movingLeft(false), movingRight(false), movingUp(false),
         movingDown(false), rotatingUp(false), rotatingDown(false), rotatingLeft(false), rotatingRight(false)
{
}

Camera::Camera(glm::vec3 position)
        :pos(position), moveSpeed(MOVE_SPEED), rotationSpeed(ROTATION_SPEED), movingState(), orientationLeftRight(0),
         orientationUpDown(0),
         fovVertical(glm::pi<float>() * 0.25f),
         aspectRatio(4.0f / 3.0f), nearDistance(0.1f), farDistance(1000.0f)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    // Start with identity matrix
    glm::mat4 view(1.0f);

    // First the rotation
    view = glm::rotate(view, orientationUpDown, glm::vec3(-1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, orientationLeftRight, glm::vec3(0.0f, 1.0f, 0.0f));

    // Then the translation
    view = glm::translate(view, -pos);

    return view;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(fovVertical, aspectRatio, nearDistance, farDistance);
}

const glm::vec3& Camera::position() const
{
    return pos;
}

void Camera::advance(float dt)
{
    glm::mat4 identity(1.0f);
    glm::mat4 rotatedUpDown = glm::rotate(identity, orientationUpDown, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 fullRotation = glm::rotate(rotatedUpDown, orientationLeftRight, glm::vec3(0.0f, -1.0f, 0.0f));

    glm::vec4 forwards = fullRotation * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 up = fullRotation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 right = fullRotation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    glm::vec4 posHomogenous(pos, 1.0f);

    if (movingState.movingForwards) {
        posHomogenous += moveSpeed * dt * forwards;
    }

    if (movingState.movingBackwards) {
        posHomogenous -= moveSpeed * dt * forwards;
    }

    if (movingState.movingLeft) {
        posHomogenous -= moveSpeed * dt * right;
    }

    if (movingState.movingRight) {
        posHomogenous += moveSpeed * dt * right;
    }

    if (movingState.movingUp) {
        posHomogenous += moveSpeed * dt * up;
    }

    if (movingState.movingDown) {
        posHomogenous -= moveSpeed * dt * up;
    }

    pos[0] = posHomogenous[0];
    pos[1] = posHomogenous[1];
    pos[2] = posHomogenous[2];

    float orientationDiff = dt * rotationSpeed;

    if (movingState.rotatingLeft) {
        orientationLeftRight = glm::mod(orientationLeftRight - orientationDiff, 2.0f * glm::pi<float>());
    }

    if (movingState.rotatingRight) {
        orientationLeftRight = glm::mod(orientationLeftRight + orientationDiff, 2.0f * glm::pi<float>());
    }

    if (movingState.rotatingUp) {
        orientationUpDown = glm::mod(orientationUpDown + orientationDiff, 2.0f * glm::pi<float>());
    }

    if (movingState.rotatingDown) {
        orientationUpDown = glm::mod(orientationUpDown - orientationDiff, 2.0f * glm::pi<float>());
    }
}

} // namespace PBR
