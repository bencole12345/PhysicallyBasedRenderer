#ifndef PHYSICALLYBASEDRENDERER_SHAPES
#define PHYSICALLYBASEDRENDERER_SHAPES

#include <memory>
#include <vector>

#include "core/VertexData.h"

namespace PBR::scene_objects {

struct Shapes {
private:
    static std::shared_ptr<VertexData> s_texturedCubeVertexData;
    static std::shared_ptr<VertexData> s_untexturedCubeVertexData;
    static std::shared_ptr<std::vector<float>> s_cubeIndices;

    static std::shared_ptr<VertexData> s_texturedPlaneVertexData;
    static std::shared_ptr<VertexData> s_untexturedPlaneVertexData;
    static std::shared_ptr<std::vector<float>> s_planeIndices;

public:
    /**
     * Obtain a pointer to vertex data for a textured cube.
     */
    static std::shared_ptr<VertexData> texturedCube();

    /**
     * Obtain a pointer to vertex data for an untextured cube.
     */
    static std::shared_ptr<VertexData> untexturedCube();

    /**
     * Obtain a pointer to vertex data for a textured plane.
     */
    static std::shared_ptr<VertexData> texturedPlane();

    /**
     * Obtain a pointer to vertex data for an untextured plane.
     */
    static std::shared_ptr<VertexData> untexturedPlane();

private:
    /**
     * Checks that the passed `vertexData` buffer has been initialised, populating it if not.
     */
    static void ensureVertexDataInitialised(std::shared_ptr<VertexData>& vertexData, const float *vertexBuffer,
                                            size_t vertexBufferLen,
                                            const unsigned int* indexBuffer, size_t indexBufferLen, bool textured);
};

} // namespace PBR::scene_objects

#endif //PHYSICALLYBASEDRENDERER_SHAPES
