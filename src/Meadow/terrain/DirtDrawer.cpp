/*!
 *  @author    Dubsky Tomas
 */
#include <glm/vec2.hpp>

#include <Meadow/constants/terrain.hpp>
#include <Meadow/terrain/DirtDrawer.hpp>
#include <Meadow/terrain/shaders/AllShaders.hpp>

namespace md {

DirtDrawer::DirtDrawer(vk::PipelineLayout pipelineLayout)
    : m_drawDirtPl(
          {.topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .cullMode           = vk::CullModeFlagBits::eBack,
           .enableDepth        = true,
           .enableBlend        = false,
           .pipelineLayout     = pipelineLayout},
          {.vert = dirt_vert, .tesc = dirt_tesc, .tese = dirt_tese, .frag = dirt_frag}
      )
    , m_debugDirtPl(
          {.topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .enableDepth        = true,
           .enableBlend        = false,
           .pipelineLayout     = pipelineLayout},
          {.vert = dirt_vert,
           .tesc = dirt_tesc,
           .tese = debugDirt_tese,
           .geom = debugNormal_geom,
           .frag = debugColor_frag}
      ) {
}

void DirtDrawer::render(const vk::CommandBuffer& cmdBuf, bool showTessellation) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_drawDirtPl);
    cmdBuf.draw(k_dirtGridSize.x * k_dirtGridSize.y, 1, 0, 0);

    if (showTessellation) {
        cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_debugDirtPl);
        cmdBuf.draw(k_dirtGridSize.x * k_dirtGridSize.y, 1, 0, 0);
    }
}

} // namespace md