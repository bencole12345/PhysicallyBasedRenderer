#include "LightSource.h"

#include <glm/vec3.hpp>

namespace PBR {

LightSource::LightSource(glm::vec3 pos, glm::vec3 colour)
: pos(pos), colour(colour)
{
}

} // namespace PBR
