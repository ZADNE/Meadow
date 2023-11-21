/*!
 *  @author    Dubsky Tomas
 */
#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/constants/time.hpp>
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

void GrassDrawer::step() {
    m_timeSec += k_perStep;
}

void GrassDrawer::prerenderCompute(
    const vk::CommandBuffer& cmdBuf,
    double                   interpolationFactor,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos
) {
    m_pc.projViewMat = projViewMat;
    m_pc.cameraPos   = glm::vec4(cameraPos, 0.0f);
    float windDir    = calculateWindDir(interpolationFactor);
    m_pc.windDir.x   = glm::cos(windDir);
    m_pc.windDir.y   = glm::sin(windDir);
    cmdBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eCompute, *m_pipelineLayout, 0, *m_descriptorSet, {}
    );
    cmdBuf.pushConstants<PushConstants>(*m_pipelineLayout, k_pcShaderStages, 0u, m_pc);
    m_stalkDrawer.prerenderCompute(cmdBuf);
}

void GrassDrawer::render(const vk::CommandBuffer& cmdBuf) {
    m_stalkDrawer.render(cmdBuf);
    m_dirtDrawer.render(cmdBuf);
}

void GrassDrawer::postrenderCompute(const vk::CommandBuffer& cmbBuf) {
    m_stalkDrawer.postrenderCompute(cmbBuf);
}

float GrassDrawer::calculateWindDir(float interpolationFactor) const {
    float windDir    = 0.0f;
    float timeScaled = (m_timeSec + interpolationFactor * 0.02f) / 128.0f;
    float amplitude  = glm::pi<float>();
    for (size_t i = 0; i < 4; i++) {
        windDir += glm::perlin(glm::vec2(timeScaled, m_pc.seed)) * amplitude;
        amplitude *= 0.5;
        timeScaled *= 2.0f;
    }

    return windDir;
}

} // namespace md