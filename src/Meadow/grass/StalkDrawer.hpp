/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/buffers/Buffer.hpp>
#include <RealEngine/graphics/descriptors/DescriptorSet.hpp>
#include <RealEngine/graphics/pipelines/Pipeline.hpp>

namespace md {

/**
 * @brief Draws grass stalks
 */
class StalkDrawer {
public:
    StalkDrawer(vk::PipelineLayout pipelineLayout, re::DescriptorSet& descriptorSet);

    void prerenderCompute(const vk::CommandBuffer& cmdBuf);

    void render(const vk::CommandBuffer& cmdBuf);

    void postrenderCompute(const vk::CommandBuffer& cmdBuf);

private:
    re::Pipeline m_prepareStalksPl;
    re::Pipeline m_drawStalksPl;
    re::Buffer   m_stalkBuf;
};

} // namespace md