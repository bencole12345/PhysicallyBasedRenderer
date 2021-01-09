#ifndef PHYSICALLYBASEDRENDERER_SKYBOXRENDERER
#define PHYSICALLYBASEDRENDERER_SKYBOXRENDERER

#include <memory>

#include "core/Camera.h"
#include "core/ShaderProgram.h"
#include "skybox/Skybox.h"

namespace PBR::skybox {

class SkyboxRenderer {
private:
    /**
     * The shader program used to render the skybox.
     */
    ShaderProgram shaderProgram;

public:
    SkyboxRenderer();

    /**
     * Renders a skybox to the scene.
     *
     * @param skybox The skybox to render
     * @param camera The camera being used to render the scene
     */
    void renderSkybox(const std::shared_ptr<Skybox>& skybox, const Camera& camera);
};

} // namespace PBR::skybox

#endif //PHYSICALLYBASEDRENDERER_SKYBOXRENDERER
