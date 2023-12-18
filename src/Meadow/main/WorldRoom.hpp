/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/graphics/cameras/FlyingCamera3D.hpp>
#include <RealEngine/graphics/cameras/View2D.hpp>
#include <RealEngine/rooms/Room.hpp>

#include <Meadow/generic/ViewFrustumDrawer.hpp>
#include <Meadow/terrain/TerrainDrawer.hpp>

namespace md {

class WorldRoom: public re::Room {
public:
    explicit WorldRoom(size_t roomName);

    void sessionStart(const re::RoomTransitionArguments& args) override;
    void sessionEnd() override;
    void step() override;
    void render(const vk::CommandBuffer& cmdBuf, double interpolationFactor) override;

    void windowResizedCallback(glm::ivec2 oldSize, glm::ivec2 newSize) override;

private:
    glm::mat4 assembleProjectionMatrix(glm::vec2 windowDims) const;

    re::FlyingCamera3D m_camera{{0.0, -2.0, 0.0}, {0.0f, 0.0f, glm::pi<float>()}};
    glm::mat4     m_projMat = assembleProjectionMatrix(engine().windowDims());
    glm::mat4     m_cullingProjMat;
    glm::vec3     m_cullingCameraPos;
    glm::vec3     m_rotation{};
    glm::vec3     m_rightUpBack{};
    TerrainDrawer m_terrainDrawer{673.0f};
    ViewFrustumDrawer m_viewFrustumDrawer;
    bool              m_hiddenCursor = true;

    // GUI options
    bool m_freezeCulling    = false;
    bool m_showTessellation = false;
    bool m_showGrassNormals = false;
};

} // namespace md