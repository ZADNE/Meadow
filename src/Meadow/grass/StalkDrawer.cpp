/*!
 *  @author    Dubsky Tomas
 */
#include <Meadow/grass/StalkDrawer.hpp>
#include <Meadow/grass/StalkSB.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

using enum vk::BufferUsageFlagBits;

using D = vk::DescriptorType;

namespace md {

namespace {
constexpr std::array k_bindings = std::to_array<vk::VertexInputBindingDescription>({{
    0u,                            // Binding index
    sizeof(Stalk),                 // Stride
    vk::VertexInputRate::eInstance // Input rate
}});
constexpr std::array k_attributes = std::to_array<vk::VertexInputAttributeDescription>(
    {{
         0u,                              // Location
         0u,                              // Binding index
         vk::Format::eR32G32B32A32Sfloat, // Format
         offsetof(Stalk, posSize)         // Relative offset
     },
     {
         1u,                        // Location
         0u,                        // Binding index
         vk::Format::eR32G32Sfloat, // Format
         offsetof(Stalk, facing)    // Relative offset
     }}
);
const vk::PipelineVertexInputStateCreateInfo k_vertexInput{
    {}, k_bindings, k_attributes};
constexpr vk::DrawIndirectCommand k_stalkIndirectCommandTemplate{13, 0, 0, 0};
} // namespace

StalkDrawer::StalkDrawer(
    vk::PipelineLayout                          pipelineLayout,
    re::FrameDoubleBuffered<re::DescriptorSet>& descriptorSet
)
    : m_prepareStalksPl({.pipelineLayout = pipelineLayout}, {.comp = prepareStalks_comp})
    , m_drawStalksPl(
          {.vertexInput    = &k_vertexInput,
           .topology       = vk::PrimitiveTopology::eTriangleStrip,
           .enableDepth    = true,
           .enableBlend    = false,
           .pipelineLayout = pipelineLayout},
          {.vert = drawStalks_vert, .frag = drawStalks_frag}
      )
    , m_stalkBuf({re::BufferCreateInfo{
          .memoryUsage = vma::MemoryUsage::eAutoPreferDevice,
          .sizeInBytes = sizeof(StalkSB),
          .usage       = eIndirectBuffer | eVertexBuffer | eStorageBuffer,
          .initData    = re::objectToByteSpan(k_stalkIndirectCommandTemplate),
          .initDataDstOffset = offsetof(StalkSB, command)}}) {
    descriptorSet.forEach([&](auto& set) {
        set.write(D::eStorageBuffer, 1, 0, m_stalkBuf, 0ull, vk::WholeSize);
    });
}

void StalkDrawer::prerenderCompute(const vk::CommandBuffer& cmdBuf) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eCompute, *m_prepareStalksPl);
    cmdBuf.dispatch(k_mapGridSize.x, k_mapGridSize.y, 1);
}

void StalkDrawer::render(const vk::CommandBuffer& cmdBuf) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_drawStalksPl);
    cmdBuf.bindVertexBuffers(0u, *m_stalkBuf, offsetof(StalkSB, stalks));
    cmdBuf.drawIndirect(
        *m_stalkBuf, offsetof(StalkSB, command), 1, sizeof(vk::DrawIndirectCommand)
    );
}

void StalkDrawer::postrenderCompute(const vk::CommandBuffer& cmdBuf) {
    // Reset the number of stalks to zero in preparation for next frame
    using Command = decltype(StalkSB::command);
    cmdBuf.fillBuffer(
        *m_stalkBuf,
        offsetof(StalkSB, command) + offsetof(Command, instanceCount),
        sizeof(Command::instanceCount),
        0
    );
}

} // namespace md