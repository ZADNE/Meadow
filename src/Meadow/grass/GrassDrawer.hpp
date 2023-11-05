/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <glm/mat4x4.hpp>

#include <RealEngine/graphics/pipelines/PipelineLayout.hpp>

#include <Meadow/grass/DirtDrawer.hpp>
#include <Meadow/grass/StalkDrawer.hpp>

namespace md {

/**
 * @brief Draws dirt with grass on top of it
 */
class GrassDrawer {
public:
    explicit GrassDrawer(float seed);

    void render(
        const vk::CommandBuffer& commandBuffer,
        double                   interpolationFactor,
        const glm::mat4&         projViewMat,
        const glm::vec3&         cameraPos
    );

private:
    struct PushConstants {
        glm::mat4 projViewMat;
        glm::vec4 cameraPos; // w unused
        float     seed;
    } m_pc{};

    re::PipelineLayout m_pipelineLayout;

    DirtDrawer  m_dirtDrawer;
    StalkDrawer m_stalkDrawer;
};

} // namespace md