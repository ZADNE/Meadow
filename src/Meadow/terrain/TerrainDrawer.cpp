/*!
 *  @author    Dubsky Tomas
 */
#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/constants/time.hpp>
#include <Meadow/terrain/TerrainDrawer.hpp>
#include <Meadow/terrain/shaders/AllShaders.hpp>

using enum vk::ShaderStageFlagBits;
using enum vk::BufferUsageFlagBits;

using D = vk::DescriptorType;

namespace md {

namespace {
constexpr vk::ShaderStageFlags k_terrainUnifferShaderStages =
    eVertex | eTessellationControl | eTessellationEvaluation | eGeometry |
    eFragment | eCompute;
constexpr re::BufferCreateInfo k_terrainStageBufferCreateInfo{
    .allocFlags = vma::AllocationCreateFlagBits::eMapped |
                  vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
    .memoryUsage = vma::MemoryUsage::eAutoPreferHost,
    .sizeInBytes = sizeof(TerrainUB),
    .usage       = eTransferSrc};
constexpr re::BufferCreateInfo k_terrainBufferCreateInfo{
    .memoryUsage = vma::MemoryUsage::eAutoPreferHost,
    .sizeInBytes = sizeof(TerrainUB),
    .usage       = eTransferDst | eUniformBuffer};
} // namespace

TerrainDrawer::TerrainDrawer(float seed)
    : m_seed(seed)
    , m_terrainStageBuf(
          re::BufferMapped<TerrainUB>{k_terrainStageBufferCreateInfo},
          re::BufferMapped<TerrainUB>{k_terrainStageBufferCreateInfo}
      )
    , m_terrainBuf(
          re::BufferMapped<TerrainUB>{k_terrainBufferCreateInfo},
          re::BufferMapped<TerrainUB>{k_terrainBufferCreateInfo}
      )
    , m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .bindings =
                  {{{0, D::eUniformBuffer, 1, k_terrainUnifferShaderStages}, // Terrain uniffer
                    {1, D::eStorageBuffer, 1, eCompute}}}, // Blade buffer
          }
      )
    , m_dirtDrawer(*m_pipelineLayout)
    , m_bladeDrawer(*m_pipelineLayout, m_descriptorSet) {

    m_descriptorSet.forEach(
        [](auto& set, auto& buf) {
            set.write(D::eUniformBuffer, 0, 0, buf, 0ull, vk::WholeSize);
        },
        m_terrainBuf
    );
}

void TerrainDrawer::step() {
    m_timeSec += k_perStep;
}

void TerrainDrawer::prerenderCompute(
    const vk::CommandBuffer& cmdBuf,
    double                   interpolationFactor,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos,
    const glm::mat4&         cullingProjViewMat,
    const glm::vec3&         cullingCameraPos
) {
    // Update uniform buffer
    float   windDir = calculateWindDir(interpolationFactor);
    TerrainUB tmp{
        .projViewMat        = projViewMat,
        .cullingProjViewMat = cullingProjViewMat,
        .cameraPos          = glm::vec4{cameraPos, 0.0f},
        .cullingCameraPos   = glm::vec4{cullingCameraPos, 0.0f},
        .windDir            = glm::vec2{glm::cos(windDir), glm::sin(windDir)},
        .seed               = m_seed};
    auto& stage = m_terrainStageBuf.write();
    std::memcpy(stage.mapped(), &tmp, sizeof(tmp));
    vk::BufferCopy2 region{0, 0, sizeof(tmp)};
    cmdBuf.copyBuffer2({stage.buffer(), *m_terrainBuf.write(), region});

    cmdBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eCompute,
        *m_pipelineLayout,
        0,
        *m_descriptorSet.read(),
        {}
    );
    m_bladeDrawer.prerenderCompute(cmdBuf);
}

void TerrainDrawer::render(const vk::CommandBuffer& cmdBuf, bool showTessellation) {
    cmdBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        *m_pipelineLayout,
        0,
        *m_descriptorSet.read(),
        {}
    );
    if (!showTessellation) {
        m_bladeDrawer.render(cmdBuf);
    }
    m_dirtDrawer.render(cmdBuf, showTessellation);
}

void TerrainDrawer::postrenderCompute(const vk::CommandBuffer& cmbBuf) {
    m_bladeDrawer.postrenderCompute(cmbBuf);
}

float TerrainDrawer::calculateWindDir(float interpolationFactor) const {
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