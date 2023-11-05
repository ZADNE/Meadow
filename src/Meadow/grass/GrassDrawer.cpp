/*!
 *  @author    Dubsky Tomas
 */
#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/grass/GrassDrawer.hpp>
#include <Meadow/grass/shaders/AllShaders.hpp>

using enum vk::ShaderStageFlagBits;
using enum vk::BufferUsageFlagBits;

using D = vk::DescriptorType;

namespace md {

namespace {
constexpr vk::ShaderStageFlags k_pcShaderStages =
    eTessellationControl | eTessellationEvaluation | eGeometry | eFragment;
constexpr auto k_test = std::to_array<vk::DrawIndirectCommand>(
    {vk::DrawIndirectCommand{3, 1, 0, 0}, vk::DrawIndirectCommand{3, 1, 0, 0}}
);
} // namespace

GrassDrawer::GrassDrawer(float seed)
    : m_pc{.seed = seed}
    , m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .bindings = {{}},
              .ranges   = {vk::PushConstantRange{
                  k_pcShaderStages, 0u, sizeof(PushConstants)}}}
      )
    , m_dirtDrawer(*m_pipelineLayout)
    , m_stalkDrawer(*m_pipelineLayout) {
}

void GrassDrawer::render(
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
    m_dirtDrawer.render(commandBuffer);
    m_stalkDrawer.render(commandBuffer);
}

} // namespace md