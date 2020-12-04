#ifndef PHYSICALLYBASEDRENDERER_CAMERA
#define PHYSICALLYBASEDRENDERER_CAMERA

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace PBR {

/**
 * Wraps all flags for whether we are currently moving.
 */
struct MovingState {

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
    MovingState();
};

class Camera {
private:

    // Position and orientation
    glm::vec3 pos;
    float orientationLeftRight;
    float orientationUpDown;

    // Movement
    float moveSpeed;
    float rotationSpeed;
    MovingState movingState;

    // Camera settings
    float fovVertical;
    float aspectRatio;
    float nearDistance;
    float farDistance;

public:
    Camera();

    glm::mat4 getMVPMatrix();

    const glm::vec3& position();

    void advance(float dt);

    void setMovingForwards(bool movingForwards);
    void setMovingBackwards(bool movingBackwards);
    void setMovingLeft(bool movingLeft);
    void setMovingRight(bool movingRight);
    void setMovingUp(bool movingUp);
    void setMovingDown(bool movingDown);

    void setRotatingUp(bool rotatingUp);
    void setRotatingDown(bool rotatingDown);
    void setRotatingLeft(bool rotatingLeft);
    void setRotatingRight(bool rotatingRight);
};

inline
void Camera::setMovingForwards(bool movingForwards)
{
    this->movingState.movingForwards = movingForwards;
}

inline
void Camera::setMovingBackwards(bool movingBackwards)
{
    this->movingState.movingBackwards = movingBackwards;
}

inline
void Camera::setMovingLeft(bool movingLeft)
{
    this->movingState.movingLeft = movingLeft;
}

inline
void Camera::setMovingRight(bool movingRight)
{
    this->movingState.movingRight = movingRight;
}

inline
void Camera::setMovingUp(bool movingUp)
{
    this->movingState.movingUp = movingUp;
}

inline
void Camera::setMovingDown(bool movingDown)
{
    this->movingState.movingDown = movingDown;
}

inline
void Camera::setRotatingUp(bool rotatingUp)
{
    this->movingState.rotatingUp = rotatingUp;
}

inline
void Camera::setRotatingDown(bool rotatingDown)
{
    this->movingState.rotatingDown = rotatingDown;
}

inline
void Camera::setRotatingLeft(bool rotatingLeft)
{
    this->movingState.rotatingLeft = rotatingLeft;
}

inline
void Camera::setRotatingRight(bool rotatingRight)
{
    this->movingState.rotatingRight = rotatingRight;
}

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_CAMERA
