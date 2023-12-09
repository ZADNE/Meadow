﻿/*!
 *  @author    Dubsky Tomas
 */
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/constants/time.hpp>
#include <Meadow/main/WorldRoom.hpp>

using namespace ImGui;

namespace md {

constexpr auto k_clearValues = std::to_array<vk::ClearValue>(
    {vk::ClearColorValue{0.25411764705f, 0.7025490196f, 0.90470588235f, 1.0f},
     vk::ClearDepthStencilValue{1.0f, 0u}}
);

WorldRoom::WorldRoom(size_t roomName)
    : re::Room{
          roomName,
          re::RoomDisplaySettings{
              .clearValues          = k_clearValues,
              .stepsPerSecond       = k_stepsPerSecond,
              .framesPerSecondLimit = k_frameRateLimit,
              .usingImGui           = true}} {
    engine().setWindowTitle("Meadow");
}

void WorldRoom::sessionStart(const re::RoomTransitionArguments& args) {
    engine().setRelativeCursorMode(m_hiddenCursor);
}

void WorldRoom::sessionEnd() {
}

void WorldRoom::step() {
    if (m_hiddenCursor) { // If not in menu
        const auto& rel     = engine().cursorRel();
        int         rollDir = (engine().isKeyDown(re::Key::E) > 0) -
                      (engine().isKeyDown(re::Key::Q) > 0);
        m_rotation = glm::vec3{
            glm::vec2{-rel.y, rel.x} * 0.003f, static_cast<float>(rollDir) * 0.01f};
        m_camera.rotate(m_rotation);

        m_rightUpBack = glm::vec3{0.0f};

        m_rightUpBack.x += (engine().isKeyDown(re::Key::D) > 0) -
                           (engine().isKeyDown(re::Key::A) > 0);
        m_rightUpBack.y += (engine().isKeyDown(re::Key::T) > 0) -
                           (engine().isKeyDown(re::Key::G) > 0);
        m_rightUpBack.z += (engine().isKeyDown(re::Key::S) > 0) -
                           (engine().isKeyDown(re::Key::W) > 0);
        float length  = glm::length(m_rightUpBack);
        m_rightUpBack = length > 0.0 ? m_rightUpBack / length : glm::vec3{0.0f};
        m_rightUpBack *= engine().isKeyDown(re::Key::LShift) ? 0.25f : 0.03125f;

        m_camera.move(m_rightUpBack);
    }

    // Switch menu on/off
    if (engine().wasKeyPressed(re::Key::LMB)) {
        engine().setRelativeCursorMode(m_hiddenCursor = !m_hiddenCursor);
    }

    m_grassDrawer.step();
}

void WorldRoom::render(const vk::CommandBuffer& cmdBuf, double interpolationFactor) {
    glm::mat4 projViewMat = m_projMat * m_camera.viewMat();

    if (!m_freezeCulling) {
        m_cullingProjMat   = projViewMat;
        m_cullingCameraPos = m_camera.pos();
    }

    // Prepare to render the
    m_grassDrawer.prerenderCompute(
        cmdBuf, interpolationFactor, projViewMat, m_camera.pos(), m_cullingProjMat, m_cullingCameraPos
    );

    // Begin main render pass
    engine().mainRenderPassBegin();

    // Render the world
    m_grassDrawer.render(cmdBuf, m_showTessellation);

    // Render UI
    if (Begin("Meadow", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (TreeNodeEx("Performance", ImGuiTreeNodeFlags_DefaultOpen)) {
            using namespace std::chrono;
            ImGui::Text(
                "FPS: %u\nMax FT: %i us",
                engine().framesPerSecond(),
                (int)duration_cast<microseconds>(engine().maxFrameTime()).count()
            );
            TreePop();
        }
        if (TreeNode("Implementation")) {
            TextUnformatted("Freeze culling");
            SameLine();
            ToggleButton("FreezeCulling", &m_freezeCulling);
            TextUnformatted("Show tessellation");
            SameLine();
            ToggleButton("##ShowTessellation", &m_showTessellation);
            TreePop();
        }
    }
    End();
    engine().mainRenderPassDrawImGui();

    // End main render pass
    engine().mainRenderPassEnd();

    // Cleanup compute work before next frame
    m_grassDrawer.postrenderCompute(cmdBuf);
}

void WorldRoom::windowResizedCallback(glm::ivec2 oldSize, glm::ivec2 newSize) {
    m_projMat = assembleProjectionMatrix(newSize);
}

glm::mat4 WorldRoom::assembleProjectionMatrix(glm::vec2 windowDims) const {
    return glm::perspective(
        glm::radians(45.0f), windowDims.x / windowDims.y, 0.1f, 1000.0f
    );
}

} // namespace md