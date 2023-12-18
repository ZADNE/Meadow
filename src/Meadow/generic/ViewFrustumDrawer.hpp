/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <glm/mat4x4.hpp>

#include <RealEngine/graphics/pipelines/Pipeline.hpp>
#include <RealEngine/graphics/pipelines/PipelineLayout.hpp>

namespace md {

/**
 * @brief Draws view frustum
 * @note This class is to be used for debug purposes
 */
class ViewFrustumDrawer {
public:
    ViewFrustumDrawer() {}

    void render(
        const vk::CommandBuffer& cmdBuf,
        const glm::mat4&         projViewMat,
        const glm::vec3&         cameraPos,
        const glm::mat4&         cullingProjViewMat,
        const glm::vec3&         cullingCameraPos
    );

private:
    re::PipelineLayout m_pipelineLayout;
    re::Pipeline       m_pipeline;
};

} // namespace md