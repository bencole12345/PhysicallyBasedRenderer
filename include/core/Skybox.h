#ifndef PHYSICALLYBASEDRENDERER_SKYBOX
#define PHYSICALLYBASEDRENDERER_SKYBOX

#include <string_view>
#include <vector>

#include "core/Camera.h"
#include "core/ShaderProgram.h"

namespace PBR {

class Skybox {
private:
    unsigned int textureId;
    ShaderProgram shaderProgram;
    unsigned int vaoId;
    unsigned int vboId;

public:
    explicit Skybox(const std::vector<std::string_view>& faceTextures);
    ~Skybox();

    void draw(const Camera& camera);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SKYBOX
