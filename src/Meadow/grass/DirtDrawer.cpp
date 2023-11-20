/*!
 *  @author    Dubsky Tomas
 */
#include <glm/vec2.hpp>

#include <Meadow/constants/map.hpp>
#include <Meadow/grass/DirtDrawer.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

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
    , m_debugPl(
          {.topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .enableDepth        = true,
           .enableBlend        = false,
           .pipelineLayout     = pipelineLayout},
          {.vert = dirt_vert,
           .tesc = dirt_tesc,
           .tese = debugDirt_tese,
           .geom = debugDirt_geom,
           .frag = debugColor_frag}
      ) {
}

void DirtDrawer::render(const vk::CommandBuffer& cmdBuf) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_drawDirtPl);
    cmdBuf.draw(k_mapGridSize.x * k_mapGridSize.y, 1, 0, 0);
    /*cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_debugPl);
    cmdBuf.draw(k_mapGridSize.x * k_mapGridSize.y, 1, 0, 0);*/
}

} // namespace md