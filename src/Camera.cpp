#include "Camera.h"

#define MOVE_SPEED 0.5
#define ROTATION_SPEED 0.3

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

namespace PBR {

MovingState::MovingState()
        :movingForwards(false), movingBackwards(false), movingLeft(false), movingRight(false), movingUp(false),
         movingDown(false), rotatingUp(false), rotatingDown(false), rotatingLeft(false), rotatingRight(false)
{
}

Camera::Camera()
        :pos(0.0f), moveSpeed(MOVE_SPEED), rotationSpeed(ROTATION_SPEED), movingState(), orientationLeftRight(0),
         orientationUpDown(0),
         fovVertical(glm::pi<float>() * 0.25f),
         aspectRatio(4.0f / 3.0f), nearDistance(0.1f), farDistance(1000.0f)
{
}

glm::mat4 Camera::getMVPMatrix()
{
    glm::mat4 Projection = glm::perspective(fovVertical, aspectRatio, nearDistance, farDistance);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), -pos);
    View = glm::rotate(View, orientationUpDown, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, orientationLeftRight, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

const glm::vec3& Camera::position()
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
