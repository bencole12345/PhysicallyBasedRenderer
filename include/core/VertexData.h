#ifndef PHYSICALLYBASEDRENDERER_VERTEXDATA
#define PHYSICALLYBASEDRENDERER_VERTEXDATA

#include <vector>

namespace PBR {

/**
 * Contains a buffer of vertex data that has been sent to the GPU.
 *
 * Supports dynamic data organisations and stride lengths.
 */
class VertexData {
private:
    std::vector<float> data;
    size_t stride;

    unsigned int vaoId;
    unsigned int vboId;

    bool hasNormals;
    bool hasTextureCoordinates;

    size_t positionsOffset;
    size_t normalsOffset;
    size_t textureCoordinatesOffset;

public:
    VertexData(const float *data, size_t bufferLength, bool textured = true);
    ~VertexData();

    VertexData(const VertexData& other) = delete;
    void operator=(const VertexData& other) = delete;

    /**
     * Whether this vertex data includes normals.
     */
    bool includesNormals() const;

    /**
     * Whether this vertex data includes texture coordinates.
     */
    bool includesTextureCoordinates() const;

    /**
     * The offset of the positions in the array of floats.
     */
    size_t getPositionsOffset() const;

    /**
     * The offset of the normals in the array of floats.
     *
     * Undefined unless the vertex data includes normals.
     */
    size_t getNormalsOffset() const;

    /**
     * The offset of the texture coordinates in the array of floats.
     *
     * Undefined unless the vertex data includes texture coordinates.
     */
    size_t getTextureCoordinatesOffset() const;

    unsigned int getVaoId() const;

    unsigned int getVboId() const;

    /**
     * The number of triangles pointed to by this data.
     */
    unsigned int trianglesCount() const;
};

inline
bool VertexData::includesNormals() const
{
    return hasNormals;
}

inline
bool VertexData::includesTextureCoordinates() const
{
    return hasTextureCoordinates;
}

inline
size_t VertexData::getPositionsOffset() const
{
    return positionsOffset;
}

inline
size_t VertexData::getNormalsOffset() const
{
    assert(hasNormals);
    return normalsOffset;
}

inline
size_t VertexData::getTextureCoordinatesOffset() const
{
    assert(hasTextureCoordinates);
    return textureCoordinatesOffset;
}

inline
unsigned int VertexData::getVaoId() const
{
    return vaoId;
}

inline
unsigned int VertexData::getVboId() const
{
    return vboId;
}

inline
unsigned int VertexData::trianglesCount() const
{
    return data.size() / stride;
}

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_VERTEXDATA
