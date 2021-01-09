#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENEOBJECT
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENEOBJECT

#include "core/SceneObject.h"
#include "physically_based/PhysicallyBasedMaterial.h"

namespace PBR::physically_based {

using PhysicallyBasedSceneObject = SceneObject<PhysicallyBasedMaterial>;

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENEOBJECT
