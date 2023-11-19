/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/cameras/FlyingCamera3D.hpp>
#include <RealEngine/graphics/cameras/View2D.hpp>
#include <RealEngine/rooms/Room.hpp>

#include <Meadow/grass/GrassDrawer.hpp>

namespace md {

class WorldRoom: public re::Room {
public:
    explicit WorldRoom(size_t roomName);

    void sessionStart(const re::RoomTransitionArguments& args) override;
    void sessionEnd() override;
    void step() override;
    void render(const vk::CommandBuffer& cmbBuf, double interpolationFactor) override;

private:
    re::FlyingCamera3D m_camera{{0.0, -2.0, 0.0}, {0.0f, 0.0f, glm::pi<float>()}};
    glm::mat4   m_projViewMat{};
    GrassDrawer m_grassDrawer{673.0f};
    bool        m_hiddenCursor = true;
};

} // namespace md