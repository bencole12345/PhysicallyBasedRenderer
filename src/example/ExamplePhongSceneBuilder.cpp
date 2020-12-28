#include "example/ExamplePhongSceneBuilder.h"

#include <memory>
#include <string>
#include <string_view>

#include "core/PointLightSource.h"
#include "core/ShaderProgram.h"
#include "core/Skybox.h"
#include "core/Texture.h"
#include "phong/PhongScene.h"
#include "phong/PhongSceneObject.h"

namespace PBR {

using namespace phong;

static float orangeCubeVertices[] = {
        // Position(x, y, z), Normal(x, y, z), Colour(r, g, b)

        // Back face
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.2f,

        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,

        // Right face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.2f,

        // Top face
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.2f,
};

static float greyPlaneVertices[] = {
        // Position(x, y, z), Normal(x, y, z), Colour(r, g, b)

        10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,

        -10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
};

static float cookieCubeVertices[] = {
        // Position(x, y, z), Normal(x, y, z), TexCoord(u, v)

        // Back face
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        // Right face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        // Top face
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
};

void ExamplePhongSceneBuilder::loadBasicPhongScene(std::shared_ptr<PhongScene>* scene)
{
    // Shader program for the untextured cube
    const std::string vertexShader = "src/shaders/phong/phong_untextured.vert";
    const std::string fragmentShader = "src/shaders/phong/phong_untextured.frag";
    std::shared_ptr<ShaderProgram> untexturedShaderProgram(new ShaderProgram(vertexShader, fragmentShader));

    // Shader program for the textured cube
    const std::string texturedVertexShader = "src/shaders/phong/phong_textured.vert";
    const std::string texturedFragmentShader = "src/shaders/phong/phong_textured.frag";
    std::shared_ptr<ShaderProgram> texturedShaderProgram(
            new ShaderProgram(texturedVertexShader, texturedFragmentShader));

    // Texture for the cube
    std::shared_ptr<Texture> texture(new Texture("resources/textures/cookie.jpg"));

    // Create the untextured cube
    float kD = 0.6f;
    float kS = 0.4f;
    float specularN = 2.0f;
    glm::vec3 orangeCubePosition(2.0f, 0.0f, 0.0f);
    glm::vec3 orangeCubeOrientation(0.0f);
    float orangeCubeScale = 1.0f;
    std::shared_ptr<PhongSceneObject> orangeCube(
            new PhongSceneObject(orangeCubeVertices, sizeof(orangeCubeVertices), untexturedShaderProgram,
                    orangeCubePosition, orangeCubeOrientation, orangeCubeScale, kD, kS, specularN));

    // Create the textured cube
    specularN = 3.0f;
    glm::vec3 cookiePosition(-2.0f, 0.0f, 0.0f);
    glm::vec3 cookieOrientation(0.5f, 0.5f, 0.5f);
    float cookieScale = 2.0f;
    std::shared_ptr<PhongSceneObject> cookieCube(
            new PhongSceneObject(cookieCubeVertices, sizeof(cookieCubeVertices), texture, texturedShaderProgram,
                    cookiePosition, cookieOrientation, cookieScale, kD, kS, specularN));

    // Create the plane
    kD = 0.8f;
    kS = 0.2f;
    specularN = 1.0f;
    glm::vec3 planePosition(0.0f, -2.0f, 0.0f);
    glm::vec3 planeOrientation(0.0f);
    float planeScale = 1.0f;
    std::shared_ptr<PhongSceneObject> plane(
            new PhongSceneObject(greyPlaneVertices, sizeof(greyPlaneVertices), untexturedShaderProgram, planePosition,
                    planeOrientation, planeScale, kD, kS, specularN));

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Set up the skybox
    std::vector<std::string_view> skyboxTextures{
            "resources/skyboxes/ocean_with_mountains/right.jpg",
            "resources/skyboxes/ocean_with_mountains/left.jpg",
            "resources/skyboxes/ocean_with_mountains/top.jpg",
            "resources/skyboxes/ocean_with_mountains/bottom.jpg",
            "resources/skyboxes/ocean_with_mountains/front.jpg",
            "resources/skyboxes/ocean_with_mountains/back.jpg"
    };
    std::shared_ptr<Skybox> skybox(new Skybox(skyboxTextures));

    // Allocate the scene
    *scene = std::make_shared<PhongScene>(backgroundColour, ambientLight);

    // Add the scene objects
    (*scene)->addSceneObject(orangeCube);
    (*scene)->addSceneObject(cookieCube);
//    (*scene)->addSceneObject(plane);

    // Add the skybox
    (*scene)->addSkybox(skybox);

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
