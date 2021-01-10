#ifndef PHYSICALLYBASEDRENDERER_ERRORCODES
#define PHYSICALLYBASEDRENDERER_ERRORCODES

namespace PBR {

enum class ErrorCodes {

    /**
     * Errors reported by GLFW
     */
    GlfwError = 1,

    /**
     * Failed to load GLEW.
     */
    GlewError = 2,

    /**
     * Failed to parse a .obj file
     */
    BadObjFile = 10,

    /**
     * Failed to load a texture file
     */
    BadTexture = 11,

    /**
     * Failed to compile a shader program
     */
    BadShaderProgram = 12,

    /**
     * Failed to link a shader program
     */
    FailedToLinkShaders = 13,
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_ERRORCODES
