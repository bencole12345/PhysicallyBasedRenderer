#include "ExamplePhongSceneBuilder.h"

#include <memory>
#include <string>

#include "core/PointLightSource.h"
#include "core/ShaderProgram.h"
#include "phong/PhongScene.h"
#include "phong/PhongSceneObject.h"

namespace PBR {

using namespace phong;

static float cubeVertices[] = {
        // Position(x, y, z), Normal(x, y, z), Colour(r, g, b)

        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,

        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
};

static float planeVertices[] = {
        // Position(x, y, z), Normal(x, y, z), Colour(r, g, b)

        10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,

        -10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
};

void ExamplePhongSceneBuilder::loadBasicPhongScene(std::shared_ptr<PhongScene>* scene)
{
    // Set up a shader program for the triangles
    const std::string vertexShader = "src/phong/shaders/vertex/phong.vert";
    const std::string fragmentShader = "src/phong/shaders/fragment/phong.frag";
    std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(vertexShader, fragmentShader));

    // Create the cube
    float kD = 0.6f;
    float kS = 0.4f;
    float specularN = 2.0f;
    std::shared_ptr<PhongSceneObject> triangle(
            new PhongSceneObject(cubeVertices, sizeof(cubeVertices), shaderProgram, kD, kS,
                    specularN));

    // Create the plane
    kD = 0.8f;
    kS = 0.2f;
    specularN = 1.0f;
    std::shared_ptr<PhongSceneObject> plane(
            new PhongSceneObject(planeVertices, sizeof(planeVertices), shaderProgram, kD, kS, specularN));

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.2f, 0.2f, 0.3f);

    // Allocate the scene
    *scene = std::make_shared<PhongScene>(backgroundColour, ambientLight);

    // Add the scene objects
    (*scene)->addSceneObject(triangle);
    (*scene)->addSceneObject(plane);

    // Add a light source (below, to the right)
    glm::vec3 lightPosition(2.0f, -1.0f, 2.0f);
    glm::vec3 lightColour(1.0f, 1.0f, 1.0f);
    std::shared_ptr<PointLightSource> light(new PointLightSource(lightPosition, lightColour));
    (*scene)->addLightSource(light);

    // Add a second light source (above, slightly to the left and behind)
    lightPosition = glm::vec3(-1.0f, 5.0f, -1.0f);
    std::shared_ptr<PointLightSource> light2(new PointLightSource(lightPosition, lightColour));
    (*scene)->addLightSource(light2);
}

} // namespace PBR
