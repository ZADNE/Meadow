/*!
 *  @author    Dubsky Tomas
 */
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/main/WorldRoom.hpp>

using namespace ImGui;

namespace md {

#ifdef _DEBUG
constexpr unsigned int k_frameRateLimit = 300u;
#else
constexpr unsigned int k_frameRateLimit = re::Synchronizer::k_doNotLimitFramesPerSecond;
#endif // _DEBUG

constexpr auto k_clearValues = std::to_array<vk::ClearValue>(
    {vk::ClearColorValue{0.25411764705f, 0.7025490196f, 0.90470588235f, 1.0f},
     vk::ClearDepthStencilValue{1.0f, 0u}}
);

WorldRoom::WorldRoom(size_t roomName)
    : re::Room{
          roomName,
          re::RoomDisplaySettings{
              .clearValues          = k_clearValues,
              .stepsPerSecond       = 50,
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
        m_camera.rotate(glm::vec3{
            glm::vec2{-rel.y, rel.x} * 0.008f,
            static_cast<float>(rollDir) * 0.01f});

        glm::vec3 rightUpBack{0.0f};

        rightUpBack.x += (engine().isKeyDown(re::Key::D) > 0) -
                         (engine().isKeyDown(re::Key::A) > 0);
        rightUpBack.y += (engine().isKeyDown(re::Key::T) > 0) -
                         (engine().isKeyDown(re::Key::G) > 0);
        rightUpBack.z += (engine().isKeyDown(re::Key::S) > 0) -
                         (engine().isKeyDown(re::Key::W) > 0);
        float length = glm::length(rightUpBack);
        rightUpBack  = length > 0.0 ? rightUpBack / length : glm::vec3{0.0f};
        rightUpBack *= engine().isKeyDown(re::Key::LShift) ? 1.0f : 0.125f;

        m_camera.move(rightUpBack);

        glm::vec2 windowDims = engine().windowDims();
        glm::mat4 projMat    = glm::perspective(
            glm::radians(45.0f), windowDims.x / windowDims.y, 0.1f, 1000.0f
        );
        m_projViewMat = projMat * m_camera.viewMat();
    }

    // Switch menu on/off
    if (engine().wasKeyPressed(re::Key::LMB)) {
        engine().setRelativeCursorMode(m_hiddenCursor = !m_hiddenCursor);
    }

    m_grassDrawer.step();
}

void WorldRoom::render(const vk::CommandBuffer& cmdBuf, double interpolationFactor) {
    m_grassDrawer.prerenderCompute(
        cmdBuf, interpolationFactor, m_projViewMat, m_camera.pos()
    );

    engine().mainRenderPassBegin();

    m_grassDrawer.render(cmdBuf);

    if (Begin("Meadow", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (BeginTabBar("##TabBar")) {
            if (BeginTabItem("Performance")) {
                using namespace std::chrono;
                ImGui::Text(
                    "FPS: %u\nMax FT: %i us",
                    engine().framesPerSecond(),
                    (int)duration_cast<microseconds>(engine().maxFrameTime()).count()
                );
                EndTabItem();
            }
            EndTabBar();
        }
    }
    End();

    engine().mainRenderPassDrawImGui();
    engine().mainRenderPassEnd();

    m_grassDrawer.postrenderCompute(cmdBuf);
}

} // namespace md