/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/buffers/Buffer.hpp>
#include <RealEngine/graphics/pipelines/Pipeline.hpp>

namespace md {

/**
 * @brief Draws grass stalks
 */
class StalkDrawer {
public:
    explicit StalkDrawer(vk::PipelineLayout pipelineLayout);

    void render(const vk::CommandBuffer& commandBuffer);

private:
    re::Pipeline m_pipeline;
    re::Buffer   m_indirectBuffer;
};

} // namespace md