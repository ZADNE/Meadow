/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/pipelines/Pipeline.hpp>

namespace md {

/**
 * @brief Draws dirt below grass stalks
 */
class DirtDrawer {
public:
    explicit DirtDrawer(vk::PipelineLayout pipelineLayout);

    void render(const vk::CommandBuffer& cmbBuf);

private:
    re::Pipeline m_drawDirtPl;
    re::Pipeline m_debugPl;
};

} // namespace md