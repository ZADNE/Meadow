/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <glm/mat4x4.hpp>

#include <RealEngine/graphics/descriptors/DescriptorSet.hpp>
#include <RealEngine/graphics/pipelines/PipelineLayout.hpp>

#include <Meadow/grass/DirtDrawer.hpp>
#include <Meadow/grass/StalkDrawer.hpp>

namespace md {

/**
 * @brief Draws dirt with grass on top of it
 */
class GrassDrawer {
public:
    explicit GrassDrawer(float seed);

    void prepareToRender(
        const vk::CommandBuffer& cmbBuf,
        double                   interpolationFactor,
        const glm::mat4&         projViewMat,
        const glm::vec3&         cameraPos
    );

    void render(const vk::CommandBuffer& cmbBuf);

private:
    struct PushConstants {
        glm::mat4 projViewMat;
        glm::vec4 cameraPos; // w unused
        float     seed;
    } m_pc{};

    re::PipelineLayout m_pipelineLayout;
    re::DescriptorSet  m_descriptorSet{m_pipelineLayout.descriptorSetLayout(0)};

    DirtDrawer  m_dirtDrawer;
    StalkDrawer m_stalkDrawer;
};

} // namespace md