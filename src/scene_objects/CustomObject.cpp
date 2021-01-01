#include "scene_objects/CustomObject.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include <boost/functional/hash.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "core/Material.h"
#include "core/Texture.h"
#include "core/VertexData.h"

namespace PBR::scene_objects {

namespace {

typedef struct vertex_data_t {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;

    bool operator==(const struct vertex_data_t& other) const
    {
        return position == other.position && normal == other.normal && texcoord == other.texcoord;
    }

} vertex_data_t;

struct VertexDataHasher {
    size_t operator()(const vertex_data_t& vertexData) const
    {
        size_t seed = 0;
        boost::hash_combine(seed, boost::hash_value(vertexData.position.x));
        boost::hash_combine(seed, boost::hash_value(vertexData.position.y));
        boost::hash_combine(seed, boost::hash_value(vertexData.position.z));
        boost::hash_combine(seed, boost::hash_value(vertexData.normal.x));
        boost::hash_combine(seed, boost::hash_value(vertexData.normal.y));
        boost::hash_combine(seed, boost::hash_value(vertexData.normal.z));
        boost::hash_combine(seed, boost::hash_value(vertexData.texcoord.s));
        boost::hash_combine(seed, boost::hash_value(vertexData.texcoord.t));
        return seed;
    }
};

std::shared_ptr<VertexData> loadObjFromPath(std::string_view objPath, bool textured)
{
    tinyobj::ObjReader reader;

    // Parse the file
    bool success = reader.ParseFromFile(objPath.data());

    // Check for errors/warnings
    if (!success) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }
    else if (!reader.Warning().empty()) {
        std::cerr << "TinyObjReader: " << reader.Warning();
    }

    // Read the data that was parsed
    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();
    const auto& materials = reader.GetMaterials();

    // Buffers to store the loaded data
    std::shared_ptr<std::vector<float>> vertexDataBuffer(new std::vector<float>());
    std::shared_ptr<std::vector<unsigned int>> indicesBuffer(new std::vector<unsigned int>());
    size_t stride = textured ? 8 : 6;

    // Explanation: We need to send to OpenGL a buffer containing all data about
    // vertices stored contiguously, e.g.
    // {... pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tex.u, tex.v ...}, but
    // tinyobjloader loads vertices, normals and texture coordinates into separate
    // buffers. We therefore need to rearrange the data so that it's contiguous,
    // adding in some redundancy as a necessary side effect.

    // Used to track which (position, normal, texcoord) triples we've already seen so
    // that the vertex can be reused. If we are processing untextured data then just
    // set the texcoord to (0,0).
    std::unordered_map<vertex_data_t, unsigned int, VertexDataHasher> indicesMap;

    // Loop over all vertices of all shapes
    for (const auto& shape : shapes) {
        for (auto idx : shape.mesh.indices) {

            // Read the data parsed by tinyobjloader
            glm::vec3 vertexPosition = *((glm::vec3*) &attrib.vertices[3 * idx.vertex_index]);
            glm::vec3 normal = *((glm::vec3*) &attrib.normals[3 * idx.normal_index]);
            glm::vec2 texCoords = textured
                                  ? *((glm::vec2*) &attrib.texcoords[2 * idx.texcoord_index])
                                  : glm::vec2(0.0f);

            // Turn it into a tuple so we can test it against the map of vertices we've
            // already seen
            vertex_data_t vertexInfo = {vertexPosition, normal, texCoords};

            // Determine whether we've already seen this vertex
            auto it = indicesMap.find(vertexInfo);
            if (it == indicesMap.end()) {
                // We haven't seen this vertex before, so copy it to the buffer

                // Compute the index for the new vertex
                size_t newIndex = vertexDataBuffer->size() / stride;
                indicesBuffer->push_back(newIndex);

                // Add it to our map
                indicesMap.insert(std::make_pair(vertexInfo, newIndex));

                // Copy the loaded data to the vertex buffer
                vertexDataBuffer->push_back(vertexPosition.x);
                vertexDataBuffer->push_back(vertexPosition.y);
                vertexDataBuffer->push_back(vertexPosition.z);
                vertexDataBuffer->push_back(normal.x);
                vertexDataBuffer->push_back(normal.y);
                vertexDataBuffer->push_back(normal.z);
                if (textured) {
                    vertexDataBuffer->push_back(texCoords.s);
                    vertexDataBuffer->push_back(texCoords.t);
                }
            }
            else {
                // We've already seen this (position, normal, texcoord) triple
                // so reuse the existing one.
                indicesBuffer->push_back(it->second);
            }
        }
    }

    return std::make_shared<VertexData>(vertexDataBuffer, indicesBuffer, textured);
}

} // anonymous namespace

CustomObject::CustomObject(std::string_view objPath, std::string_view texturePath, glm::vec3 pos, glm::vec3 orientation,
                           const Material& material, float scale)
        :SceneObject(pos, orientation, glm::vec3(scale), material, loadObjFromPath(objPath, true),
                     std::shared_ptr<Texture>(new Texture(texturePath)))
{
}

CustomObject::CustomObject(std::string_view objPath, glm::vec3 colour, glm::vec3 pos, glm::vec3 orientation,
                           const Material& material, float scale)
        :SceneObject(pos, orientation, glm::vec3(scale), material, loadObjFromPath(objPath, false), colour)
{
}

} // namespace PBR::scene_objects
