#ifndef PHYSICALLYBASEDRENDERER_DEBUGGINGUTIL
#define PHYSICALLYBASEDRENDERER_DEBUGGINGUTIL

#include <memory>

#include "core/Texture.h"

namespace PBR::debug {

void renderTextureToBottomCorner(std::shared_ptr<Texture> texture, bool isHDR, bool useMipmapSampling = false,
                                 float lod = 0.0f);

} // namespace PBR::debug

#endif //PHYSICALLYBASEDRENDERER_DEBUGGINGUTIL
