#ifndef PHYSICALLYBASEDRENDERER_EXAMPLESCENEBUILDER
#define PHYSICALLYBASEDRENDERER_EXAMPLESCENEBUILDER

#include <memory>

#include "core/Scene.h"

namespace PBR {

/**
 * Constructs example scenes.
 */
struct ExampleSceneBuilder {
    static void loadBasicScene(std::shared_ptr<Scene>* scene);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_EXAMPLESCENEBUILDER
