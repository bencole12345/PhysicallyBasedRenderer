#ifndef PHYSICALLYBASEDRENDERER_EXAMPLEPHONGSCENEBUILDER
#define PHYSICALLYBASEDRENDERER_EXAMPLEPHONGSCENEBUILDER

#include <memory>

#include "phong/PhongScene.h"

namespace PBR {

struct ExamplePhongSceneBuilder {
    static void loadBasicPhongScene(std::shared_ptr<phong::PhongScene> *scene);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_EXAMPLEPHONGSCENEBUILDER
