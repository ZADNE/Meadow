/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <glm/mat4x4.hpp>

#include <RealEngine/graphics/pipelines/Pipeline.hpp>
#include <RealEngine/graphics/pipelines/PipelineLayout.hpp>

namespace md {

/**
 * @brief Is a silly view frustum visualizer
 * @note This class is to be used for debug purposes only
 */
class ViewFrustumDrawer {
public:
    ViewFrustumDrawer();

    void render(
        const vk::CommandBuffer& cmdBuf,
        const glm::mat4&         projViewMat,
        const glm::mat4&         cullingProjViewMat
    );

private:
    struct ViewFrustumPC {
        glm::mat4 projViewMat;
        glm::mat4 ndcToWorldMat;
    } m_pc{};

    re::PipelineLayout m_pipelineLayout;
    re::Pipeline       m_pipeline;
};

} // namespace md