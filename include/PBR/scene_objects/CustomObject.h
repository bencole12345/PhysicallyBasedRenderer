#ifndef PHYSICALLYBASEDRENDERER_CUSTOMOBJECT
#define PHYSICALLYBASEDRENDERER_CUSTOMOBJECT

#include <string_view>

#include <glm/vec3.hpp>

#include "core/SceneObject.h"
#include "core/VertexData.h"

namespace PBR::scene_objects {

/**
 * A custom `SceneObject` with a mesh loaded from a .obj file.
 */
template<class MaterialType>
class CustomObject : public SceneObject<MaterialType> {
public:
    /**
     * Construct an object from a .obj file.
     *
     * The object will be considered textured iff a value is supplied for `texture`.
     *
     * @param objPath The path to the .obj file
     * @param pos The position of the object
     * @param orientation The orientation of the object
     * @param material The object's material
     * @param scale The scale of the object
     * @param texture (Optional) The texture of the object
     */
    CustomObject(std::string_view objPath, glm::vec3 pos, glm::vec3 orientation, const MaterialType& material,
                 float scale, const std::optional<std::shared_ptr<Texture>>& texture = std::nullopt);
};

/**
 * Loads vertex data from the specified .obj file.
 *
 * @param objPath The path to the file
 * @param textured Whether the object will be textured
 * @return A pointer to the `VertexData` object created
 */
std::shared_ptr<VertexData> loadObjFromPath(std::string_view objPath, bool textured);

template<class MaterialType>
CustomObject<MaterialType>::CustomObject(std::string_view objPath, glm::vec3 pos, glm::vec3 orientation,
                                         const MaterialType& material, float scale,
                                         const std::optional<std::shared_ptr<Texture>>& texture)
        : SceneObject<MaterialType>(pos, orientation, glm::vec3(scale), material,
                                    loadObjFromPath(objPath, texture.has_value()), texture)
{ }

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_CUSTOMOBJECT
