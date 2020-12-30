#ifndef PHYSICALLYBASEDRENDERER_PLANE
#define PHYSICALLYBASEDRENDERER_PLANE

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "core/Material.h"
#include "core/SceneObject.h"
#include "core/Texture.h"
#include "core/VertexData.h"

namespace PBR::scene_objects {

/**
 * A plane that lazily instantiates and shares the same vertex data
 * between all instantiations.
 */
class Plane : public SceneObject {
private:
    static std::shared_ptr<VertexData> s_texturedVertexData;
    static std::shared_ptr<VertexData> s_untexturedVertexData;
    static std::shared_ptr<VertexData> getTexturedVertexData();
    static std::shared_ptr<VertexData> getUntexturedVertexData();

public:
    /**
     * Create a textured plane.
     */
     Plane(glm::vec3 pos, glm::vec3 orientation, glm::vec2 dimensions, Material material, const std::shared_ptr<Texture>& texture);

     /**
      * Create a plane of constant colour.
      */
     Plane(glm::vec3 pos, glm::vec3 orientation, glm::vec2 dimensions, Material material, glm::vec3 colour);

};

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_PLANE
