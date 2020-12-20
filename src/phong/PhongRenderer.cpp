#include "PhongRenderer.h"

#include <utility>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/Camera.h"

namespace PBR {
namespace phong {

PhongRenderer::PhongRenderer(const std::shared_ptr<PhongScene>& scene)
    : scene(scene)
{
    // Use the Z buffer
    glEnable(GL_DEPTH_TEST);

    // Cull faces oriented the wrong way for performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_FACE);
    glFrontFace(GL_CCW);

    scene->activate();
}

PhongRenderer::~PhongRenderer() noexcept = default;

void PhongRenderer::render(const Camera& camera, double time)
{
    scene->render(camera, time);
}

} // namespace phong
} // namespace PBR
