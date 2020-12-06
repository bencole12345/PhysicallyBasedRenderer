#ifndef PHYSICALLYBASEDRENDERER_EXAMPLEPHONGSCENEBUILDER
#define PHYSICALLYBASEDRENDERER_EXAMPLEPHONGSCENEBUILDER

#include "../phong/BasicPhongScene.h"

namespace PBR {

struct ExamplePhongSceneBuilder {
    static void loadBasicPhongScene(std::shared_ptr<BasicPhongScene> *scene);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_EXAMPLEPHONGSCENEBUILDER
