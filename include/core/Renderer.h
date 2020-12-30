#ifndef PHYSICALLYBASEDRENDERER_RENDERER
#define PHYSICALLYBASEDRENDERER_RENDERER

#include <memory>

#include "core/Camera.h"
#include "core/Scene.h"

namespace PBR {

/**
 * Interface for a generic renderer that renders a scene to the window.
 */
class Renderer {
public:
    virtual ~Renderer() = 0;

    /**
     * Call any code required to set up the renderer.
     */
    virtual void activate() = 0;

    /**
     * Render the scene.
     *
     * @param scene The scene to render
     * @param camera The current Camera
     * @param time The current time
     */
    virtual void render(std::shared_ptr<Scene> scene, const Camera& camera, double time) = 0;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_RENDERER
