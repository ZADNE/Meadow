/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/pipelines/Pipeline.hpp>

namespace md {

/**
 * @brief Draws dirt below grass blades
 */
class DirtDrawer {
public:
    explicit DirtDrawer(vk::PipelineLayout pipelineLayout);

    void render(const vk::CommandBuffer& cmdBuf, bool showTessellation);

private:
    re::Pipeline m_drawDirtPl;
    re::Pipeline m_debugDirtPl;
};

} // namespace md