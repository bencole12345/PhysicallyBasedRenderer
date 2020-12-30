#ifndef PHYSICALLYBASEDRENDERER_CAMERA
#define PHYSICALLYBASEDRENDERER_CAMERA

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace PBR {

/**
 * A camera at some position and orientation in world space,
 * capable of producing view and projection matrices.
 */
class Camera {
private:

    // Position and orientation
    glm::vec3 pos;
    float orientationLeftRight;
    float orientationUpDown;

    // Camera parameters
    float fovVertical;
    float aspectRatio;
    float nearDistance;
    float farDistance;

public:
    Camera(glm::vec3 position, float aspectRatio);

    /**
     * Sets the aspect ratio for rendering. (width / height)
     */
    void setAspectRatio(float aspectRatio);

    /**
     * Computes the view matrix of the camera.
     */
    glm::mat4 getViewMatrix() const;

    /**
     * Computes the projection matrix of the camera.
     */
    glm::mat4 getProjectionMatrix() const;

    /**
     * The current position of the camera.
     */
    const glm::vec3& position() const;

    void moveForwards(float distance);
    void moveBackwards(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);

    void rotateLeft(float angle);
    void rotateRight(float angle);
    void rotateUp(float angle);
    void rotateDown(float angle);

private:

    /**
     * Computes the rotation matrix for the camera's current orientation.
     */
    glm::mat4 rotationMatrix() const;

    /**
     * Computes the 'forwards' vector relative to the camera's orientation.
     */
    glm::vec3 forwardsVector() const;

    /**
     * Computes the 'up' vector relative to the camera's orientation.
     */
    glm::vec3 upVector() const;

    /**
     * Computes the 'right' vector relative to the camera's orientation.
     * @return
     */
    glm::vec3 rightVector() const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_CAMERA
