/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <glm/gtc/noise.hpp>
#include <glm/mat4x4.hpp>

#include <RealEngine/graphics/buffers/BufferMapped.hpp>
#include <RealEngine/graphics/descriptors/DescriptorSet.hpp>
#include <RealEngine/graphics/pipelines/PipelineLayout.hpp>

#include <Meadow/grass/DirtDrawer.hpp>
#include <Meadow/grass/GrassUB.hpp>
#include <Meadow/grass/StalkDrawer.hpp>

namespace md {

/**
 * @brief Draws dirt with grass on top of it
 */
class GrassDrawer {
public:
    explicit GrassDrawer(float seed);

    void step();

    void prerenderCompute(
        const vk::CommandBuffer& cmdBuf,
        double                   interpolationFactor,
        const glm::mat4&         projViewMat,
        const glm::vec3&         cameraPos,
        const glm::mat4&         cullingProjViewMat,
        const glm::vec3&         cullingCameraPos
    );

    void render(const vk::CommandBuffer& cmdBuf, bool showTessellation);

    void postrenderCompute(const vk::CommandBuffer& cmdBuf);

private:
    float m_seed    = 0.0f;
    float m_timeSec = 0.0f;
    float calculateWindDir(float interpolationFactor) const;

    re::PipelineLayout                         m_pipelineLayout;
    re::FrameDoubleBuffered<re::DescriptorSet> m_descriptorSet{
        re::DescriptorSet{m_pipelineLayout.descriptorSetLayout(0)},
        re::DescriptorSet{m_pipelineLayout.descriptorSetLayout(0)}};
    re::FrameDoubleBuffered<re::BufferMapped<GrassUB>> m_grassStageBuf;
    re::FrameDoubleBuffered<re::Buffer>                m_grassBuf;

    DirtDrawer  m_dirtDrawer;
    StalkDrawer m_stalkDrawer;
};

} // namespace md