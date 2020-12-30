#ifndef PHYSICALLYBASEDRENDERER_CUBE
#define PHYSICALLYBASEDRENDERER_CUBE

#include <memory>

#include <glm/vec3.hpp>

#include "core/Material.h"
#include "core/SceneObject.h"
#include "core/Texture.h"
#include "core/VertexData.h"

namespace PBR::scene_objects {

/**
 * A cube that lazily instantiates and shares the same vertex data
 * between all instances.
 */
class Cube : public SceneObject {
private:
    static std::shared_ptr<VertexData> s_texturedVertexData;
    static std::shared_ptr<VertexData> s_untexturedVertexData;
    static std::shared_ptr<VertexData> getTexturedVertexData();
    static std::shared_ptr<VertexData> getUntexturedVertexData();

public:
    /**
     * Create a textured cube.
     */
    Cube(glm::vec3 pos, glm::vec3 orientation, float scale, Material material, const std::shared_ptr<Texture>& texture);

    /**
     * Create an untextured cube.
     */
    Cube(glm::vec3 pos, glm::vec3 orientation, float scale, Material material, glm::vec3 colour);
};

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_CUBE
