/*!
 *  @author    Dubsky Tomas
 */
#include <Meadow/generic/ViewFrustumDrawer.hpp>
#include <Meadow/generic/shaders/AllShaders.hpp>

using enum vk::ShaderStageFlagBits;

namespace md {

namespace {
constexpr vk::ShaderStageFlags k_pcShaderStages = eTessellationControl |
                                                  eTessellationEvaluation;
} // namespace

ViewFrustumDrawer::ViewFrustumDrawer()
    : m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .ranges = {{k_pcShaderStages, 0u, sizeof(ViewFrustumPC)}}}
      )
    , m_pipeline(
          {.topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 4,
           .polygonMode        = vk::PolygonMode::eLine,
           .enableDepth        = true,
           .enableBlend        = false,
           .pipelineLayout     = *m_pipelineLayout},
          {.vert = debugViewFrustum_vert,
           .tesc = debugViewFrustum_tesc,
           .tese = debugViewFrustum_tese,
           .frag = debugColor_frag}
      ) {
}

void ViewFrustumDrawer::render(
    const vk::CommandBuffer& cmdBuf,
    const glm::mat4&         projViewMat,
    const glm::mat4&         cullingProjViewMat
) {
    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);

    m_pc.projViewMat   = projViewMat;
    m_pc.ndcToWorldMat = glm::inverse(cullingProjViewMat);
    cmdBuf.pushConstants<ViewFrustumPC>(*m_pipelineLayout, k_pcShaderStages, 0u, m_pc);

    // Draw 5 faces of the view frustum (not the near plane)
    cmdBuf.draw(4, 5, 0, 0);
}

} // namespace md