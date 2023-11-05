/*!
 *  @author    Dubsky Tomas
 */
#include <Meadow/grass/StalkDrawer.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

using enum vk::BufferUsageFlagBits;

namespace md {

namespace {
constexpr auto k_test = std::to_array<vk::DrawIndirectCommand>(
    {vk::DrawIndirectCommand{3, 1, 0, 0}, vk::DrawIndirectCommand{3, 1, 0, 0}}
);
} // namespace

StalkDrawer::StalkDrawer(vk::PipelineLayout pipelineLayout)
    : m_pipeline(
          {.pipelineLayout = pipelineLayout,
           .topology       = vk::PrimitiveTopology::eTriangleStrip,
           .enableDepth    = true,
           .enableBlend    = false},
          {.vert = stalk_vert, .frag = stalk_frag}
      )
    , m_indirectBuffer({re::BufferCreateInfo{
          .memoryUsage = vma::MemoryUsage::eAutoPreferDevice,
          .sizeInBytes = 2 * sizeof(vk::DrawIndirectCommand),
          .usage       = eStorageBuffer | eIndirectBuffer,
          .initData    = re::objectToByteSpan(k_test)}}) {
}

void StalkDrawer::render(const vk::CommandBuffer& commandBuffer) {
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
    commandBuffer.drawIndirect(*m_indirectBuffer, 0, 2, sizeof(vk::DrawIndirectCommand));
}

} // namespace md