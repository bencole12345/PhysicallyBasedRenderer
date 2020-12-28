#include "core/Camera.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace PBR {

Camera::Camera(glm::vec3 position, float aspectRatio)
        :pos(position), orientationLeftRight(0), orientationUpDown(0), fovVertical(glm::pi<float>() * 0.25f),
         aspectRatio(aspectRatio), nearDistance(0.1f), farDistance(1000.0f)
{
}

void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
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

void Camera::moveForwards(float distance)
{
    pos += distance * forwardsVector();
}

void Camera::moveBackwards(float distance)
{
    pos -= distance * forwardsVector();
}

void Camera::moveLeft(float distance)
{
    pos -= distance * rightVector();
}

void Camera::moveRight(float distance)
{
    pos += distance * rightVector();
}

void Camera::moveUp(float distance)
{
    pos += distance * upVector();
}

void Camera::moveDown(float distance)
{
    pos -= distance * upVector();
}

void Camera::rotateLeft(float angle)
{
    orientationLeftRight = glm::mod(orientationLeftRight - angle, 2.0f * glm::pi<float>());
}

void Camera::rotateRight(float angle)
{
    orientationLeftRight = glm::mod(orientationLeftRight + angle, 2.0f * glm::pi<float>());
}

void Camera::rotateUp(float angle)
{
    if (orientationUpDown + angle < 0.5f * glm::pi<float>()) {
        orientationUpDown += angle;
    }
}

void Camera::rotateDown(float angle)
{
    if (orientationUpDown - angle > -0.5 * glm::pi<float>()) {
        orientationUpDown -= angle;
    }
}

glm::mat4 Camera::rotationMatrix() const
{
    glm::mat4 identity(1.0f);
    glm::mat4 rotatedUpDown = glm::rotate(identity, orientationUpDown, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 fullRotation = glm::rotate(rotatedUpDown, orientationLeftRight, glm::vec3(0.0f, -1.0f, 0.0f));
    return fullRotation;
}

glm::vec3 Camera::forwardsVector() const
{
    glm::vec4 forwardsBeforeRotation(0.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 rotated = rotationMatrix() * forwardsBeforeRotation;
    return glm::vec3(rotated.x, rotated.y, rotated.z);
}

glm::vec3 Camera::rightVector() const
{
    glm::vec4 rightBeforeRotation(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 rotated = rotationMatrix() * rightBeforeRotation;
    return glm::vec3(rotated.x, rotated.y, rotated.z);
}

glm::vec3 Camera::upVector() const
{
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

} // namespace PBR
