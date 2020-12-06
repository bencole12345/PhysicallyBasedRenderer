#include "ExamplePhongSceneBuilder.h"

#include <memory>

#include <glm/vec4.hpp>

namespace PBR {

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
        10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,

        -10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        10.0f, -2.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
        -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f,
};

void ExamplePhongSceneBuilder::loadBasicPhongScene(std::shared_ptr<BasicPhongScene>* scene)
{
    // Set up a shader program for the triangles
    std::shared_ptr<Shader> vertexShader(new Shader("src/phong/shaders/vertex/phong.vert", GL_VERTEX_SHADER));
    std::shared_ptr<Shader> fragmentShader(new Shader("src/phong/shaders/fragment/phong.frag", GL_FRAGMENT_SHADER));
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
    std::shared_ptr<PhongSceneObject> plane(new PhongSceneObject(planeVertices, sizeof(planeVertices), shaderProgram, kD, kS, specularN));

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.2f, 0.2f, 0.3f);

    // Allocate the scene
    *scene = std::make_shared<BasicPhongScene>(backgroundColour, ambientLight);

    // Add the scene objects
    (*scene)->addSceneObject(triangle);
    (*scene)->addSceneObject(plane);

    // Add a light source
    glm::vec3 lightPosition(2.0f, -1.0f, 2.0f);
    glm::vec3 lightColour(1.0f, 1.0f, 1.0f);
    std::shared_ptr<LightSource> light(new LightSource(lightPosition, lightColour));
    (*scene)->addLightSource(light);
}

}; // namespace PBR
