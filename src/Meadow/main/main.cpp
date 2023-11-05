/*!
 *  @author    Dubsky Tomas
 */
#include <RealEngine/program/MainProgram.hpp>

#include <Meadow/main/WorldRoom.hpp>

using enum vk::PipelineStageFlagBits;

using A = vk::AccessFlagBits;

int main(int argc, char* argv[]) {
    // Vulkan features to enable
    auto chain = vk::StructureChain{
        vk::PhysicalDeviceFeatures2{
            vk::PhysicalDeviceFeatures{}
                .setTessellationShader(true)
                .setGeometryShader(true)
                .setFillModeNonSolid(true)
                .setFillModeNonSolid(true)
                .setMultiDrawIndirect(true)},
        vk::PhysicalDeviceVulkan11Features{}.setShaderDrawParameters(true)};

    // Buffers to be used as render targets
    auto additionalBuffers = std::to_array<re::VulkanInitInfo::BufferDescr>(
        {{vk::Format::eD32Sfloat,
          vk::ImageUsageFlagBits::eDepthStencilAttachment,
          vk::ImageAspectFlagBits::eDepth}}
    );

    // Default renderpass to be used
    auto attachmentDescriptions = std::to_array<vk::AttachmentDescription2>(
        {vk::AttachmentDescription2{
             // Output color attachment
             {},
             re::k_surfaceFormat.format,
             vk::SampleCountFlagBits::e1,
             vk::AttachmentLoadOp::eClear,     // Color
             vk::AttachmentStoreOp::eStore,    // Color
             vk::AttachmentLoadOp::eDontCare,  // Stencil
             vk::AttachmentStoreOp::eDontCare, // Stencil
             vk::ImageLayout::eUndefined,      // Initial
             vk::ImageLayout::ePresentSrcKHR   // Final
         },
         vk::AttachmentDescription2{
             {},
             vk::Format::eD32Sfloat,
             vk::SampleCountFlagBits::e1,
             vk::AttachmentLoadOp::eClear,                   // Depth
             vk::AttachmentStoreOp::eDontCare,               // Depth
             vk::AttachmentLoadOp::eDontCare,                // Stencil
             vk::AttachmentStoreOp::eDontCare,               // Stencil
             vk::ImageLayout::eUndefined,                    // Initial
             vk::ImageLayout::eDepthStencilAttachmentOptimal // Final
         }}
    );
    auto colorAttachmentRefs = std::to_array<vk::AttachmentReference2>(
        {{0, vk::ImageLayout::eColorAttachmentOptimal}}
    );
    vk::AttachmentReference2 depthAttachmentRef{
        1, vk::ImageLayout::eDepthStencilAttachmentOptimal};
    vk::SubpassDescription2 subpassDescription{
        {},
        vk::PipelineBindPoint::eGraphics,
        0u,
        {},                  // Input attachments
        colorAttachmentRefs, // Color attachments
        {},                  // Resolve attachments
        &depthAttachmentRef};
    vk::SubpassDependency2 subpassDependency{
        vk::SubpassExternal,                          // Src subpass
        0u,                                           // Dst subpass
        eColorAttachmentOutput | eEarlyFragmentTests, // Src stage mask
        eColorAttachmentOutput | eEarlyFragmentTests, // Dst stage mask
        vk::AccessFlags{},                            // Src access mask
        A::eColorAttachmentWrite | A::eDepthStencilAttachmentWrite // Dst access mask
    };
    vk::RenderPassCreateInfo2 renderPassCreateInfo{
        {}, attachmentDescriptions, subpassDescription, subpassDependency};

    // Initialize RealEngine
    re::MainProgram::initialize(
        {.deviceCreateInfoChain = &chain.get<>(),
         .defaultRenderPass     = &renderPassCreateInfo,
         .additionalBuffers     = additionalBuffers}
    );

    // Add room with the world
    re::MainProgram::addRoom<md::WorldRoom>(0);

    // Run the Meadow!
    return re::MainProgram::run(0, {});
}