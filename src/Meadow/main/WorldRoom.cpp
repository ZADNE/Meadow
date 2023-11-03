/*!
 *  @author    Dubsky Tomas
 */
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include <Meadow/main/WorldRoom.hpp>

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
}

void WorldRoom::sessionStart(const re::RoomTransitionArguments& args) {
    engine().setRelativeCursorMode(true);
}

void WorldRoom::sessionEnd() {
}

void WorldRoom::step() {
    const auto& rel     = engine().cursorRel();
    int         rollDir = (engine().isKeyDown(re::Key::E) > 0) -
                  (engine().isKeyDown(re::Key::Q) > 0);
    m_camera.rotate(glm::vec3{
        glm::vec2{-rel.y, rel.x} * 0.008f, static_cast<float>(rollDir) * 0.01f});

    glm::vec3 leftUpForward{0.0f};

    leftUpForward.x += (engine().isKeyDown(re::Key::A) > 0) -
                       (engine().isKeyDown(re::Key::D) > 0);
    leftUpForward.z += (engine().isKeyDown(re::Key::W) > 0) -
                       (engine().isKeyDown(re::Key::S) > 0);
    leftUpForward *= 0.125f;

    m_camera.move(leftUpForward);

    glm::vec2 windowDims = engine().windowDims();
    glm::mat4 projMat    = glm::perspective(
        glm::radians(45.0f), windowDims.x / windowDims.y, 0.1f, 1000.0f
    );
    m_projViewMat = projMat * m_camera.viewMat();
}

void WorldRoom::render(
    const vk::CommandBuffer& commandBuffer, double interpolationFactor
) {
    m_dirtDrawer.render(
        commandBuffer, interpolationFactor, m_projViewMat, m_camera.pos()
    );
}

} // namespace md