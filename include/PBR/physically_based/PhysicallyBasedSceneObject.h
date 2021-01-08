#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENEOBJECT
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENEOBJECT

#include "core/SceneObject.h"
#include "physically_based/PhysicallyBasedMaterial.h"

namespace PBR::physically_based {

typedef SceneObject<PhysicallyBasedMaterial> PhysicallyBasedSceneObject;

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENEOBJECT
