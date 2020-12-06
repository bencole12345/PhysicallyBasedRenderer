#ifndef PHYSICALLYBASEDRENDERER_SCENE
#define PHYSICALLYBASEDRENDERER_SCENE

#include "Camera.h"

namespace PBR {

class Scene {
public:
    virtual void render(Camera& camera, double time) = 0;
    virtual void activate() = 0;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SCENE
