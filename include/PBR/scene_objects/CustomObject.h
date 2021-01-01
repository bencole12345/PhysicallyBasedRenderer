#ifndef PHYSICALLYBASEDRENDERER_CUSTOMOBJECT
#define PHYSICALLYBASEDRENDERER_CUSTOMOBJECT

#include <string_view>

#include <glm/vec3.hpp>

#include "core/Material.h"
#include "core/SceneObject.h"
#include "core/VertexData.h"

namespace PBR::scene_objects {

/**
 * A custom `SceneObject` with a mesh loaded from a .obj file.
 */
class CustomObject : public SceneObject {
public:
    /**
     * Load a textured object from a .obj file.
     *
     * @param objPath The path to the .obj file
     * @param texturePath The path to the texture file
     * @param pos The position of the object, in world coordinates
     * @param orientation The orientation of the objects, in world space
     * @param material The material of the object
     * @param scale The scale of the object
     */
    CustomObject(std::string_view objPath, std::string_view texturePath, glm::vec3 pos, glm::vec3 orientation,
                 const Material& material, float scale = 1.0f);

    /**
     * Load an untextured object from a .obj file.
     *
     * @param objPath The path to the .obj file
     * @param texturePath The path to the texture file
     * @param pos The position of the object, in world coordinates
     * @param orientation The orientation of the objects, in world space
     * @param material The material of the object
     * @param scale The scale of the object
     */
    CustomObject(std::string_view objPath, glm::vec3 colour, glm::vec3 pos, glm::vec3 orientation,
                 const Material& material, float scale = 1.0f);
};

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_CUSTOMOBJECT
