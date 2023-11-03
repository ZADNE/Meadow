/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <glm/mat4x4.hpp>

#include <RealEngine/graphics/pipelines/Pipeline.hpp>
#include <RealEngine/graphics/pipelines/PipelineLayout.hpp>

namespace md {

/**
 * @brief Draws dirt below grass stalks
 */
class DirtDrawer {
public:
    explicit DirtDrawer(float seed);

    void render(
        const vk::CommandBuffer& commandBuffer,
        double                   interpolationFactor,
        const glm::mat4&         projViewMat,
        const glm::vec3&         cameraPos
    );

private:
    struct PushConstants {
        glm::mat4 projViewMat;
        glm::vec4 cameraPos; // w unused
        float     seed;
    } m_pc{};

    re::PipelineLayout m_pipelineLayout;
    re::Pipeline       m_pipeline;
    re::Pipeline       m_debugPipeline;
};

} // namespace md