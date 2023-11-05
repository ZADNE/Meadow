﻿/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/pipelines/Pipeline.hpp>

namespace md {

/**
 * @brief Draws dirt below grass stalks
 */
class DirtDrawer {
public:
    explicit DirtDrawer(vk::PipelineLayout pipelineLayout);

    void render(const vk::CommandBuffer& commandBuffer);

private:
    re::Pipeline m_pipeline;
    re::Pipeline m_debugPipeline;
};

} // namespace md