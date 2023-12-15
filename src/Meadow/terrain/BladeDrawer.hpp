/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/buffers/Buffer.hpp>
#include <RealEngine/graphics/descriptors/DescriptorSet.hpp>
#include <RealEngine/graphics/pipelines/Pipeline.hpp>
#include <RealEngine/graphics/synchronization/DoubleBuffered.hpp>

namespace md {

/**
 * @brief Draws terrain blades
 */
class BladeDrawer {
public:
    BladeDrawer(
        vk::PipelineLayout                          pipelineLayout,
        re::FrameDoubleBuffered<re::DescriptorSet>& descriptorSet
    );

    void prerenderCompute(const vk::CommandBuffer& cmdBuf);

    void render(const vk::CommandBuffer& cmdBuf, bool showGrassNormals);

    void postrenderCompute(const vk::CommandBuffer& cmdBuf);

private:
    re::Pipeline m_prepareBladesPl;
    re::Pipeline m_drawBladesPl;
    re::Pipeline m_drawNormalsPl;
    re::Buffer   m_bladeBuf;
};

} // namespace md