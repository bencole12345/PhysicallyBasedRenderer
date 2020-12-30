#ifndef PHYSICALLYBASEDRENDERER_SCENE
#define PHYSICALLYBASEDRENDERER_SCENE

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include "core/PointLightSource.h"
#include "core/SceneObject.h"
#include "core/Skybox.h"

namespace PBR {

/**
 * Contains all information describing a renderer-independent scene.
 */
class Scene {
private:

    /**
     * The objects in the scene.
     */
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;

    /**
     * The ambient light colour.
     */
    glm::vec3 ambientLight;

    /**
     * The lights in the scene.
     */
    std::vector<PointLightSource> lights;

    /**
     * List of light positions, stored contiguously so that they can be passed
     * easily as a uniform to a shader.
     */
    std::vector<glm::vec3> lightPositions;

    /**
     * List of light colours, stored contiguously so that they can be passed
     * easily as a uniform to a shader.
     */
    std::vector<glm::vec3> lightColours;

    /**
     * The background colour. Will default to black if no value has been set.
     */
    glm::vec3 backgroundColour;

    /**
     * The skybox. Will only be rendered if a value has been provided.
     */
    std::optional<std::shared_ptr<Skybox>> skybox;

public:

    /**
     * Create a scene without a skybox.
     *
     * @param sceneObjects The objects in the scene
     * @param ambientLight The ambient light colour
     * @param lights The lights in the scene
     * @param backgroundColour The background colour of the scene
     */
    Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects,
            glm::vec3 ambientLight,
            std::vector<PointLightSource>  lights,
            glm::vec3 backgroundColour);

    /**
     * Create a scene with a skybox.
     *
     * @param sceneObjects The objects in the scene
     * @param ambientLight The ambient light colour
     * @param lights The lights in the scene
     * @param skybox The skybox to use
     */
    Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects,
            glm::vec3 ambientLight,
            std::vector<PointLightSource>  lights,
            std::shared_ptr<Skybox> skybox);

    const std::vector<std::shared_ptr<SceneObject>>& getSceneObjectsList() const;
    void addObject(const std::shared_ptr<SceneObject>& object);

    glm::vec3 getAmbientLight() const;
    const std::vector<PointLightSource>& getLights() const;
    const std::vector<glm::vec3>& getLightPositions() const;
    const std::vector<glm::vec3>& getLightColours() const;
    void addLight(const PointLightSource& light);

    glm::vec3 getBackgroundColour() const;

    bool hasSkybox() const;
    std::shared_ptr<Skybox> getSkybox() const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SCENE