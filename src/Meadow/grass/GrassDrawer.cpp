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
    eVertex | eTessellationControl | eTessellationEvaluation | eGeometry |
    eFragment | eCompute;
constexpr auto k_test = std::to_array<vk::DrawIndirectCommand>(
    {vk::DrawIndirectCommand{3, 1, 0, 0}, vk::DrawIndirectCommand{3, 1, 0, 0}}
);
} // namespace

GrassDrawer::GrassDrawer(float seed)
    : m_pc{.seed = seed}
    , m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .bindings = {{{0, D::eStorageBuffer, 1, eCompute}}}, // Stalk buffer
              .ranges = {vk::PushConstantRange{
                  k_pcShaderStages, 0u, sizeof(PushConstants)}}}
      )
    , m_dirtDrawer(*m_pipelineLayout)
    , m_stalkDrawer(*m_pipelineLayout, m_descriptorSet) {
}

void GrassDrawer::prepareToRender(
    const vk::CommandBuffer& cmbBuf,
    double                   interpolationFactor,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos
) {
    m_pc.projViewMat = projViewMat;
    m_pc.cameraPos   = glm::vec4(cameraPos, 0.0f);
    cmbBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eCompute, *m_pipelineLayout, 0, *m_descriptorSet, {}
    );
    cmbBuf.pushConstants<PushConstants>(*m_pipelineLayout, k_pcShaderStages, 0u, m_pc);
    m_stalkDrawer.prepareToRender(cmbBuf);
}

void GrassDrawer::render(const vk::CommandBuffer& cmbBuf) {
    m_stalkDrawer.render(cmbBuf);
    m_dirtDrawer.render(cmbBuf);
}

} // namespace md