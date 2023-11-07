/*!
 *  @author    Dubsky Tomas
 */
#include <glm/vec2.hpp>

#include <Meadow/grass/StalkDrawer.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

using enum vk::BufferUsageFlagBits;

namespace md {

namespace {
constexpr auto k_test = std::to_array<vk::DrawIndirectCommand>(
    {vk::DrawIndirectCommand{3, 1, 0, 0}, vk::DrawIndirectCommand{3, 1, 0, 0}}
);
constexpr glm::ivec2 k_stalkTileCount{128, 128};
} // namespace

StalkDrawer::StalkDrawer(vk::PipelineLayout pipelineLayout)
    : m_prepareStalksPl({.pipelineLayout = pipelineLayout}, {.comp = stalk_comp})
    , m_drawStalksPl(
          {.topology       = vk::PrimitiveTopology::eTriangleStrip,
           .enableDepth    = true,
           .enableBlend    = false,
           .pipelineLayout = pipelineLayout},
          {.vert = stalk_vert, .frag = stalk_frag}
      )
    , m_indirectBuf({re::BufferCreateInfo{
          .memoryUsage = vma::MemoryUsage::eAutoPreferDevice,
          .sizeInBytes = k_stalkTileCount.x * k_stalkTileCount.y *
                         sizeof(vk::DrawIndirectCommand),
          .usage = eStorageBuffer | eIndirectBuffer}}) {
}

void StalkDrawer::render(const vk::CommandBuffer& commandBuffer) {
    // commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute,
    // *m_prepareStalksPl); commandBuffer.dispatch(1, 1, 1);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_drawStalksPl);
    commandBuffer.drawIndirect(
        *m_indirectBuf,
        0,
        k_stalkTileCount.x * k_stalkTileCount.y,
        sizeof(vk::DrawIndirectCommand)
    );
}

} // namespace md