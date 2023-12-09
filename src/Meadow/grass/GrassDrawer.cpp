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
constexpr vk::ShaderStageFlags k_grassUnifferShaderStages =
    eVertex | eTessellationControl | eTessellationEvaluation | eGeometry |
    eFragment | eCompute;
constexpr re::BufferCreateInfo k_grassStageBufferCreateInfo{
    .allocFlags = vma::AllocationCreateFlagBits::eMapped |
                  vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
    .memoryUsage = vma::MemoryUsage::eAutoPreferHost,
    .sizeInBytes = sizeof(GrassUB),
    .usage       = eTransferSrc};
constexpr re::BufferCreateInfo k_grassBufferCreateInfo{
    .memoryUsage = vma::MemoryUsage::eAutoPreferHost,
    .sizeInBytes = sizeof(GrassUB),
    .usage       = eTransferDst | eUniformBuffer};
} // namespace

GrassDrawer::GrassDrawer(float seed)
    : m_seed(seed)
    , m_grassStageBuf(
          re::BufferMapped<GrassUB>{k_grassStageBufferCreateInfo},
          re::BufferMapped<GrassUB>{k_grassStageBufferCreateInfo}
      )
    , m_grassBuf(
          re::BufferMapped<GrassUB>{k_grassBufferCreateInfo},
          re::BufferMapped<GrassUB>{k_grassBufferCreateInfo}
      )
    , m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .bindings =
                  {{{0, D::eUniformBuffer, 1, k_grassUnifferShaderStages}, // Grass uniffer
                    {1, D::eStorageBuffer, 1, eCompute}}}, // Stalk buffer
          }
      )
    , m_dirtDrawer(*m_pipelineLayout)
    , m_stalkDrawer(*m_pipelineLayout, m_descriptorSet) {

    m_descriptorSet.forEach(
        [](auto& set, auto& buf) {
            set.write(D::eUniformBuffer, 0, 0, buf, 0ull, vk::WholeSize);
        },
        m_grassBuf
    );
}

void GrassDrawer::step() {
    m_timeSec += k_perStep;
}

void GrassDrawer::prerenderCompute(
    const vk::CommandBuffer& cmdBuf,
    double                   interpolationFactor,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos,
    const glm::mat4&         cullingProjViewMat,
    const glm::vec3&         cullingCameraPos
) {
    // Update uniform buffer
    float   windDir = calculateWindDir(interpolationFactor);
    GrassUB tmp{
        .projViewMat        = projViewMat,
        .cullingProjViewMat = cullingProjViewMat,
        .cameraPos          = glm::vec4{cameraPos, 0.0f},
        .cullingCameraPos   = glm::vec4{cullingCameraPos, 0.0f},
        .windDir            = glm::vec2{glm::cos(windDir), glm::sin(windDir)},
        .seed               = m_seed};
    auto& stage = m_grassStageBuf.write();
    std::memcpy(stage.mapped(), &tmp, sizeof(tmp));
    vk::BufferCopy2 region{0, 0, sizeof(tmp)};
    cmdBuf.copyBuffer2({stage.buffer(), *m_grassBuf.write(), region});

    cmdBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eCompute,
        *m_pipelineLayout,
        0,
        *m_descriptorSet.read(),
        {}
    );
    m_stalkDrawer.prerenderCompute(cmdBuf);
}

void GrassDrawer::render(const vk::CommandBuffer& cmdBuf, bool showTessellation) {
    cmdBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        *m_pipelineLayout,
        0,
        *m_descriptorSet.read(),
        {}
    );
    m_stalkDrawer.render(cmdBuf);
    m_dirtDrawer.render(cmdBuf, showTessellation);
}

void GrassDrawer::postrenderCompute(const vk::CommandBuffer& cmbBuf) {
    m_stalkDrawer.postrenderCompute(cmbBuf);
}

float GrassDrawer::calculateWindDir(float interpolationFactor) const {
    float windDir    = 0.0f;
    float timeScaled = (m_timeSec + interpolationFactor * 0.02f) / 128.0f;
    float amplitude  = glm::pi<float>();
    for (size_t i = 0; i < 4; i++) {
        windDir += glm::perlin(glm::vec2(timeScaled, m_seed)) * amplitude;
        amplitude *= 0.5;
        timeScaled *= 2.0f;
    }

    return windDir;
}

} // namespace md