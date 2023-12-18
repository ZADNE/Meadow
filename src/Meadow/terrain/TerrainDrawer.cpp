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
constexpr vk::ShaderStageFlags k_dirtUnifferShaderStages =
    eVertex | eTessellationControl | eTessellationEvaluation | eGeometry |
    eFragment | eCompute;
constexpr re::BufferCreateInfo k_dirtStageBufferCreateInfo{
    .allocFlags = vma::AllocationCreateFlagBits::eMapped |
                  vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
    .memoryUsage = vma::MemoryUsage::eAutoPreferHost,
    .sizeInBytes = sizeof(TerrainUB),
    .usage       = eTransferSrc};
constexpr re::BufferCreateInfo k_dirtBufferCreateInfo{
    .memoryUsage = vma::MemoryUsage::eAutoPreferHost,
    .sizeInBytes = sizeof(TerrainUB),
    .usage       = eTransferDst | eUniformBuffer};
} // namespace

TerrainDrawer::TerrainDrawer(float seed)
    : m_seed(seed)
    , m_terrainStageBuf(
          re::BufferMapped<TerrainUB>{k_dirtStageBufferCreateInfo},
          re::BufferMapped<TerrainUB>{k_dirtStageBufferCreateInfo}
      )
    , m_terrainBuf(
          re::BufferMapped<TerrainUB>{k_dirtBufferCreateInfo},
          re::BufferMapped<TerrainUB>{k_dirtBufferCreateInfo}
      )
    , m_pipelineLayout(
          {},
          re::PipelineLayoutDescription{
              .bindings =
                  {{{0, D::eUniformBuffer, 1, k_dirtUnifferShaderStages}, // Terrain uniffer
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
    m_windOffset += calculateWindDir() * k_perStep * 8.0f;
}

void TerrainDrawer::prerenderCompute(
    const vk::CommandBuffer& cmdBuf,
    float                    interpolationFactor,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos,
    const glm::mat4&         cullingProjViewMat,
    const glm::vec3&         cullingCameraPos,
    bool                     limitBladeCount
) {
    // Update uniform buffer
    glm::vec2 windOffset = m_windOffset +
                           calculateWindDir() * k_perStep * interpolationFactor;
    TerrainUB tmp{
        .projViewMat        = projViewMat,
        .cullingProjViewMat = cullingProjViewMat,
        .cameraPos          = glm::vec4{cameraPos, 0.0f},
        .cullingCameraPos   = glm::vec4{cullingCameraPos, 0.0f},
        .windOffset         = windOffset,
        .seed               = m_seed,
        .timeSec            = m_timeSec + interpolationFactor * k_perStep,
        .maxBladesPerTile   = limitBladeCount ? 4u : 2048u};
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

void TerrainDrawer::render(
    const vk::CommandBuffer& cmdBuf, bool showTessellation, bool showGrassNormals
) {
    cmdBuf.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        *m_pipelineLayout,
        0,
        *m_descriptorSet.read(),
        {}
    );
    if (!showTessellation) {
        m_bladeDrawer.render(cmdBuf, showGrassNormals);
    }
    m_dirtDrawer.render(cmdBuf, showTessellation);
}

void TerrainDrawer::postrenderCompute(const vk::CommandBuffer& cmbBuf) {
    m_bladeDrawer.postrenderCompute(cmbBuf);
}

glm::vec2 TerrainDrawer::calculateWindDir() const {
    float windRad    = 0.0f;
    float timeScaled = m_timeSec / 256.0f;
    float amplitude  = glm::pi<float>();
    for (size_t i = 0; i < 4; i++) {
        windRad += glm::perlin(glm::vec2(timeScaled, m_seed)) * amplitude;
        amplitude *= 0.5;
        timeScaled *= 2.0f;
    }

    return glm::vec2{glm::cos(windRad), glm::sin(windRad)};
}

} // namespace md