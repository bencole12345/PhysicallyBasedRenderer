#ifndef PHYSICALLYBASEDRENDERER_PHONGRENDERER
#define PHYSICALLYBASEDRENDERER_PHONGRENDERER

#include <memory>

#include "core/Renderer.h"
#include "core/Scene.h"
#include "core/ShaderProgram.h"
#include "phong/PhongScene.h"
#include "phong/SkyboxRenderer.h"

namespace PBR::phong {

/**
 * An implementation of the `Renderer` protocol that uses the Phong
 * shading model.
 */
class PhongRenderer : public Renderer<PhongScene> {
private:

    /**
     * Shader used to render textured scene objects.
     */
    ShaderProgram texturedObjectShader;

    /**
     * Shader used to render non-textured scene objects.
     */
    ShaderProgram nonTexturedObjectShader;

    /**
     * Used for rendering skyboxes.
     */
    phong::SkyboxRenderer skyboxRenderer;

public:
    PhongRenderer();

    void activate() override;

    void render(std::shared_ptr<PhongScene> scene, const Camera& camera, double time) override;
};

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGRENDERER
