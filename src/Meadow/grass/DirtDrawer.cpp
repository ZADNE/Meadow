/*!
 *  @author    Dubsky Tomas
 */
#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/grass/DirtDrawer.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

using enum vk::ShaderStageFlagBits;

using D = vk::DescriptorType;

namespace md {

namespace {
constexpr vk::ShaderStageFlags k_pcShaderStages =
    eTessellationControl | eTessellationEvaluation | eGeometry | eFragment;
}

DirtDrawer::DirtDrawer(float seed)
    : m_pc{.seed = seed}
    , m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .bindings = {{}},
              .ranges   = {vk::PushConstantRange{
                  k_pcShaderStages, 0u, sizeof(PushConstants)}}}
      )
    , m_pipeline(
          {.pipelineLayout     = *m_pipelineLayout,
           .topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .cullMode           = vk::CullModeFlagBits::eBack,
           .enableDepth        = true,
           .enableBlend        = false},
          {.vert = dirt_vert, .tesc = dirt_tesc, .tese = dirt_tese, .frag = dirt_frag}
      )
    , m_debugPipeline(
          {.pipelineLayout     = *m_pipelineLayout,
           .topology           = vk::PrimitiveTopology::ePatchList,
           .patchControlPoints = 1,
           .enableDepth        = true,
           .enableBlend        = false},
          {.vert = dirt_vert,
           .tesc = dirt_tesc,
           .tese = debugDirt_tese,
           .geom = debugDirt_geom,
           .frag = debugColor_frag}
      ) {
}

void DirtDrawer::render(
    const vk::CommandBuffer& commandBuffer,
    double                   interpolationFactor,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos
) {
    m_pc.projViewMat = projViewMat;
    m_pc.cameraPos   = glm::vec4(cameraPos, 0.0f);
    commandBuffer.pushConstants<PushConstants>(
        *m_pipelineLayout, k_pcShaderStages, 0u, m_pc
    );
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
    commandBuffer.draw(128 * 128, 1, 0, 0);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_debugPipeline);
    commandBuffer.draw(128 * 128, 1, 0, 0);
}

} // namespace md