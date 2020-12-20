#ifndef PHYSICALLYBASEDRENDERER_RENDERER
#define PHYSICALLYBASEDRENDERER_RENDERER

#include "Camera.h"

namespace PBR {

/**
 * Interface for a generic renderer that renders a scene to the window.
 */
class Renderer {
public:
    virtual ~Renderer() = 0;

    /**
     * Render the scene.
     *
     * @param camera The current Camera
     * @param time The current time
     */
    virtual void render(const Camera& camera, double time) = 0;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_RENDERER
