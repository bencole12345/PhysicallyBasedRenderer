#ifndef PHYSICALLYBASEDRENDERER_SCENE
#define PHYSICALLYBASEDRENDERER_SCENE

#include <memory>
#include <vector>

#include <glm/vec3.hpp>

#include "core/PointLightSource.h"
#include "skybox/Skybox.h"

namespace PBR {

/**
 * Contains all information describing a renderer-independent scene.
 */
template<class SceneObjectType>
class Scene {
private:
    /**
     * The objects in the scene.
     */
    std::vector<std::shared_ptr<SceneObjectType>> sceneObjects;

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
     * List of light intensities, stored contiguously so that they can be
     * passed easily as a uniform to a shader.
     */
    std::vector<float> lightIntensitites;

    /**
     * The background colour. Will default to black if no value has been set.
     */
    glm::vec3 backgroundColour;

public:

    /**
     * Create a scene.
     *
     * @param sceneObjects The objects in the scene
     * @param lights The lights in the scene
     * @param backgroundColour The background colour of the scene
     */
    Scene(std::vector<std::shared_ptr<SceneObjectType>> sceneObjects,
          std::vector<PointLightSource> lights,
          glm::vec3 backgroundColour = glm::vec3(0.0f));

    const std::vector<std::shared_ptr<SceneObjectType>>& getSceneObjectsList() const;

    const std::vector<PointLightSource>& getLights() const;
    const std::vector<glm::vec3>& getLightPositions() const;
    const std::vector<glm::vec3>& getLightColours() const;
    const std::vector<float>& getLightIntensities() const;

    glm::vec3 getBackgroundColour() const;
};

template<class SceneObjectType>
Scene<SceneObjectType>::Scene(std::vector<std::shared_ptr<SceneObjectType>> sceneObjects,
                              std::vector<PointLightSource> lights, glm::vec3 backgroundColour)
        :sceneObjects(std::move(sceneObjects)),
         lights(std::move(lights)),
         lightPositions(),
         lightColours(),
         backgroundColour(backgroundColour)
{
    for (const PointLightSource& light : this->lights) {
        lightPositions.push_back(light.pos);
        lightColours.push_back(light.colour);
        lightIntensitites.push_back(light.intensity);
    }
}

template<class SceneObjectType>
const std::vector<std::shared_ptr<SceneObjectType>>& Scene<SceneObjectType>::getSceneObjectsList() const
{
    return sceneObjects;
}

template<class SceneObjectType>
const std::vector<PointLightSource>& Scene<SceneObjectType>::getLights() const
{
    return lights;
}

template<class SceneObjectType>
const std::vector<glm::vec3>& Scene<SceneObjectType>::getLightPositions() const
{
    return lightPositions;
}

template<class SceneObjectType>
const std::vector<glm::vec3>& Scene<SceneObjectType>::getLightColours() const
{
    return lightColours;
}

template<class SceneObjectType>
const std::vector<float>& Scene<SceneObjectType>::getLightIntensities() const
{
    return lightIntensitites;
}

template<class SceneObjectType>
glm::vec3 Scene<SceneObjectType>::getBackgroundColour() const
{
    return backgroundColour;
}

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SCENE
