#ifndef PHYSICALLYBASEDRENDERER_PHONGRENDERER
#define PHYSICALLYBASEDRENDERER_PHONGRENDERER

#include <memory>
#include <utility>

#include "core/Renderer.h"
#include "phong/PhongScene.h"

namespace PBR {
namespace phong {

class PhongRenderer : public Renderer {
    std::shared_ptr<PhongScene> scene;

public:
    explicit PhongRenderer(const std::shared_ptr<PhongScene>& scene);
    ~PhongRenderer() noexcept override;

    void render(const Camera& camera, double time) override;
};

} // namespace phong
} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_PHONGRENDERER
