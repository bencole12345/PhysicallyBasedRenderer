#ifndef PHYSICALLYBASEDRENDERER_PHONGRENDERER
#define PHYSICALLYBASEDRENDERER_PHONGRENDERER

#include <memory>

#include "core/Renderer.h"
#include "core/Scene.h"
#include "core/ShaderProgram.h"

namespace PBR::phong {

/**
 * An implementation of the `Renderer` protocol that uses the Phong
 * shading model.
 */
class PhongRenderer : public Renderer {
private:

    /**
     * Shader used to render textured scene objects.
     */
    ShaderProgram texturedObjectShader;

    /**
     * Shader used to render non-textured scene objects.
     */
    ShaderProgram nonTexturedObjectShader;

    // TODO: Consider putting a skybox shader here too
    // (should the skybox rendering logic be implementation-specific?)

public:
    PhongRenderer();

    void activate() override;

    void render(std::shared_ptr<Scene> scene, const Camera& camera, double time) override;
};

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGRENDERER
