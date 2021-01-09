#ifndef PHYSICALLYBASEDRENDERER_PLANE
#define PHYSICALLYBASEDRENDERER_PLANE

#include <memory>
#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "core/SceneObject.h"
#include "core/Texture.h"
#include "core/VertexData.h"
#include "scene_objects/Shapes.h"

namespace PBR::scene_objects {

/**
 * A plane that lazily instantiates and shares the same vertex data
 * between all instantiations.
 */
template<class MaterialType>
class Plane : public SceneObject<MaterialType> {
public:
    /**
     * Construct a plane.
     *
     * @param pos The position of the centre of the plane in world space
     * @param orientation The orientation of the plane
     * @param dimensions The width and height of the plane
     * @param material The material of the plane
     * @param texture (Optional) The texture of the plane
     */
    Plane(glm::vec3 pos, glm::vec3 orientation, glm::vec2 dimensions, MaterialType material,
          const std::optional<std::shared_ptr<Texture>>& texture = std::nullopt);

};

template<class MaterialType>
Plane<MaterialType>::Plane(glm::vec3 pos, glm::vec3 orientation, glm::vec2 dimensions, MaterialType material,
                           const std::optional<std::shared_ptr<Texture>>& texture)
        : SceneObject<MaterialType>(pos, orientation, glm::vec3(dimensions.x, 1.0f, dimensions.y), material,
                                    texture.has_value() ? Shapes::texturedPlane() : Shapes::untexturedPlane(), texture)
{ }

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_PLANE
