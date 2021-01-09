#ifndef PHYSICALLYBASEDRENDERER_ERRORCODES
#define PHYSICALLYBASEDRENDERER_ERRORCODES

namespace PBR {

enum class ErrorCodes {

    /**
     * Errors reported by GLFW
     */
    GlfwError = 1,

    /**
     * Failed to parse a .obj file
     */
    BadObjFile,

    /**
     * Failed to load a texture file
     */
    BadTexture,

    /**
     * Failed to compile a shader program
     */
    BadShaderProgram,

    /**
     * Failed to link a shader program
     */
    FailedToLinkShaders,
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_ERRORCODES
