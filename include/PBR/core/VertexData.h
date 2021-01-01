#ifndef PHYSICALLYBASEDRENDERER_VERTEXDATA
#define PHYSICALLYBASEDRENDERER_VERTEXDATA

#include <memory>
#include <vector>

namespace PBR {

/**
 * Contains a buffer of vertex data that has been sent to the GPU.
 *
 * Supports dynamic data organisations and stride lengths.
 */
class VertexData {
private:
    std::shared_ptr<std::vector<float>> data;
    std::shared_ptr<std::vector<unsigned int>> indices;
    size_t stride;

    unsigned int vaoId;
    unsigned int vboId;
    unsigned int eboId;

    bool hasNormals;
    bool hasTextureCoordinates;

    size_t positionOffset;
    size_t normalsOffset;
    size_t textureCoordinatesOffset;

public:
    VertexData(std::shared_ptr<std::vector<float>> vertexData, std::shared_ptr<std::vector<unsigned int>> elementData,
               bool textured = true);
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

    unsigned int getEboId() const;

    /**
     * The number of triangles pointed to by this data.
     */
    unsigned int trianglesCount() const;

    /**
     * The number of vertices pointed to by this data.
     */
    unsigned int verticesCount() const;

private:
    void initBuffers();
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
    return positionOffset;
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
unsigned int VertexData::getEboId() const
{
    return eboId;
}

inline
unsigned int VertexData::trianglesCount() const
{
    return indices->size() / 3;
}

inline
unsigned int VertexData::verticesCount() const
{
    return indices->size();
}

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_VERTEXDATA
