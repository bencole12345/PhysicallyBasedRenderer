#include "PointLightSource.h"

#include <glm/vec3.hpp>

namespace PBR {

PointLightSource::PointLightSource(glm::vec3 pos, glm::vec3 colour)
: pos(pos), colour(colour)
{
}

} // namespace PBR
