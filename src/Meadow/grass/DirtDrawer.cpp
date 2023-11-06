/*!
 *  @author    Dubsky Tomas
 */
#include <glm/vec2.hpp>

#include <Meadow/grass/DirtDrawer.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

namespace md {

namespace {
constexpr glm::ivec2 k_dirtTileCount{128, 128};
} // namespace

DirtDrawer::DirtDrawer(vk::PipelineLayout pipelineLayout)
    : m_drawDirtPl(
          {.pipelineLayout     = pipelineLayout,
           .topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .cullMode           = vk::CullModeFlagBits::eBack,
           .enableDepth        = true,
           .enableBlend        = false},
          {.vert = dirt_vert, .tesc = dirt_tesc, .tese = dirt_tese, .frag = dirt_frag}
      )
    , m_debugPl(
          {.pipelineLayout     = pipelineLayout,
           .topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .enableDepth        = true,
           .enableBlend        = false},
          {.vert = dirt_vert,
           .tesc = dirt_tesc,
           .tese = debugDirt_tese,
           .geom = debugDirt_geom,
           .frag = debugColor_frag}
      ) {
}

void DirtDrawer::render(const vk::CommandBuffer& commandBuffer) {
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_drawDirtPl);
    commandBuffer.draw(k_dirtTileCount.x * k_dirtTileCount.y, 1, 0, 0);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_debugPl);
    commandBuffer.draw(k_dirtTileCount.x * k_dirtTileCount.y, 1, 0, 0);
}

} // namespace md