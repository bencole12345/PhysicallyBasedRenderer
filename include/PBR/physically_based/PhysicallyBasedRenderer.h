#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDRENDERER
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDRENDERER

#include <memory>

#include "core/Camera.h"
#include "core/Renderer.h"
#include "core/ShaderProgram.h"
#include "physically_based/EnvironmentMapRenderer.h"
#include "physically_based/PhysicallyBasedScene.h"

namespace PBR::physically_based {

/**
 * An implementation of the `Renderer` protocol that uses a physically
 * based shading model.
 */
class PhysicallyBasedRenderer : public Renderer<PhysicallyBasedScene> {
private:
    ShaderProgram shaderProgram;
    EnvironmentMapRenderer environmentMapRenderer;

public:
    PhysicallyBasedRenderer();

    void activate() override;

    void render(std::shared_ptr<PhysicallyBasedScene> scene, const Camera& camera, double time) override;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDRENDERER
