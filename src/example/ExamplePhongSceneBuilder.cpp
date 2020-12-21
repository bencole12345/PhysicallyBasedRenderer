#include "example/ExamplePhongSceneBuilder.h"

#include <memory>
#include <string>

#include "core/PointLightSource.h"
#include "core/ShaderProgram.h"
#include "core/Texture.h"
#include "phong/PhongScene.h"
#include "phong/PhongSceneObject.h"

namespace PBR {

using namespace phong;

static float orangeCubeVertices[] = {
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

static float greyPlaneVertices[] = {
        // Position(x, y, z), Normal(x, y, z), Colour(r, g, b)

        10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,

        -10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
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
    // Set up a shader program for the triangles
    const std::string vertexShader = "src/phong/shaders/vertex/phong_untextured.vert";
    const std::string fragmentShader = "src/phong/shaders/fragment/phong_untextured.frag";
    std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(vertexShader, fragmentShader));

    const std::string texturedVertexShader = "src/phong/shaders/vertex/phong_textured.vert";
    const std::string texturedFragmentShader = "src/phong/shaders/fragment/phong_textured.frag";
    std::shared_ptr<ShaderProgram> texturedShaderProgram(
            new ShaderProgram(texturedVertexShader, texturedFragmentShader));
    std::shared_ptr<Texture> texture(new Texture("resources/textures/cookie.jpg"));

    // Create the cube
    float kD = 0.6f;
    float kS = 0.4f;
    float specularN = 2.0f;
//    std::shared_ptr<PhongSceneObject> triangle(
//            new PhongSceneObject(orangeCubeVertices, sizeof(orangeCubeVertices), shaderProgram, kD, kS,
//                    specularN));

    std::shared_ptr<PhongSceneObject> cookieCube(
            new PhongSceneObject(cookieCubeVertices, sizeof(cookieCubeVertices), texture, texturedShaderProgram, kD, kS,
                    specularN));

    // Create the plane
    kD = 0.8f;
    kS = 0.2f;
    specularN = 1.0f;
    std::shared_ptr<PhongSceneObject> plane(
            new PhongSceneObject(greyPlaneVertices, sizeof(greyPlaneVertices), shaderProgram, kD, kS, specularN));

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.2f, 0.2f, 0.3f);

    // Allocate the scene
    *scene = std::make_shared<PhongScene>(backgroundColour, ambientLight);

    // Add the scene objects
//    (*scene)->addSceneObject(triangle);
    (*scene)->addSceneObject(cookieCube);
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
