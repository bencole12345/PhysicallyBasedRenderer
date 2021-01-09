#ifndef PHYSICALLYBASEDRENDERER_CUBE
#define PHYSICALLYBASEDRENDERER_CUBE

#include <memory>
#include <optional>

#include <glm/vec3.hpp>

#include "core/SceneObject.h"
#include "core/Texture.h"
#include "core/VertexData.h"
#include "scene_objects/Shapes.h"

namespace PBR::scene_objects {

/**
 * A cube that lazily instantiates and shares the same vertex data
 * between all instances.
 */
template<class MaterialType>
class Cube : public SceneObject<MaterialType> {
public:
    /**
     * Construct a cube
     *
     * @param pos The position of the centre of the cube in world space
     * @param orientation The orientation of the cube
     * @param scale The size of the sides of the cube
     * @param material The material of the cube
     * @param texture (Optional) The texture of the cube
     */
    Cube(glm::vec3 pos, glm::vec3 orientation, float scale, MaterialType material,
         const std::optional<std::shared_ptr<Texture>>& texture = std::nullopt);
};

template<class MaterialType>
Cube<MaterialType>::Cube(glm::vec3 pos, glm::vec3 orientation, float scale, MaterialType material,
                         const std::optional<std::shared_ptr<Texture>>& texture)
        : SceneObject<MaterialType>(pos, orientation, glm::vec3(scale), material,
                                    texture.has_value() ? Shapes::texturedCube() : Shapes::untexturedCube(), texture)
{ }

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_CUBE
