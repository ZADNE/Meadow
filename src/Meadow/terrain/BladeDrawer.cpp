﻿/*!
 *  @author    Dubsky Tomas
 */
#include <Meadow/terrain/BladeDrawer.hpp>
#include <Meadow/terrain/BladeSB.hpp>
#include <Meadow/terrain/shaders/AllShaders.hpp>

using enum vk::BufferUsageFlagBits;

using D = vk::DescriptorType;

namespace md {

namespace {
constexpr std::array k_bindings = std::to_array<vk::VertexInputBindingDescription>({{
    0u,                            // Binding index
    sizeof(Blade),                 // Stride
    vk::VertexInputRate::eInstance // Input rate
}});
constexpr std::array k_attributes = std::to_array<vk::VertexInputAttributeDescription>(
    {{
         0u,                              // Location
         0u,                              // Binding index
         vk::Format::eR32G32B32A32Sfloat, // Format
         offsetof(Blade, posSize)         // Relative offset
     },
     {
         1u,                        // Location
         0u,                        // Binding index
         vk::Format::eR32G32Sfloat, // Format
         offsetof(Blade, facing)    // Relative offset
     }}
);
const vk::PipelineVertexInputStateCreateInfo k_vertexInput{
    {}, k_bindings, k_attributes};
constexpr vk::DrawIndirectCommand k_bladeIndirectCommandTemplate{13, 0, 0, 0};
} // namespace

BladeDrawer::BladeDrawer(
    vk::PipelineLayout                          pipelineLayout,
    re::FrameDoubleBuffered<re::DescriptorSet>& descriptorSet
)
    : m_prepareBladesPl({.pipelineLayout = pipelineLayout}, {.comp = blades_comp})
    , m_drawBladesPl(
          {.vertexInput    = &k_vertexInput,
           .topology       = vk::PrimitiveTopology::eTriangleStrip,
           .enableDepth    = true,
           .enableBlend    = false,
           .pipelineLayout = pipelineLayout},
          {.vert = blades_vert, .frag = blades_frag}
      )
    , m_bladeBuf({re::BufferCreateInfo{
          .memoryUsage = vma::MemoryUsage::eAutoPreferDevice,
          .sizeInBytes = sizeof(BladeSB),
          .usage       = eIndirectBuffer | eVertexBuffer | eStorageBuffer,
          .initData    = re::objectToByteSpan(k_bladeIndirectCommandTemplate),
          .initDataDstOffset = offsetof(BladeSB, command)}}) {
    descriptorSet.forEach([&](auto& set) {
        set.write(D::eStorageBuffer, 1, 0, m_bladeBuf, 0ull, vk::WholeSize);
    });
}

void BladeDrawer::prerenderCompute(const vk::CommandBuffer& cmdBuf) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eCompute, *m_prepareBladesPl);
    cmdBuf.dispatch(k_mapGridSize.x, k_mapGridSize.y, 1);
}

void BladeDrawer::render(const vk::CommandBuffer& cmdBuf) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_drawBladesPl);
    cmdBuf.bindVertexBuffers(0u, *m_bladeBuf, offsetof(BladeSB, blades));
    cmdBuf.drawIndirect(
        *m_bladeBuf, offsetof(BladeSB, command), 1, sizeof(vk::DrawIndirectCommand)
    );
}

void BladeDrawer::postrenderCompute(const vk::CommandBuffer& cmdBuf) {
    // Reset the number of blades to zero in preparation for next frame
    using Command = decltype(BladeSB::command);
    cmdBuf.fillBuffer(
        *m_bladeBuf,
        offsetof(BladeSB, command) + offsetof(Command, instanceCount),
        sizeof(Command::instanceCount),
        0
    );
}

} // namespace md