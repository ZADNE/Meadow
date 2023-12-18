/*!
 *  @author    Dubsky Tomas
 */
#include <Meadow/generic/ViewFrustumDrawer.hpp>

namespace md {

void ViewFrustumDrawer::render(
    const vk::CommandBuffer& cmdBuf,
    const glm::mat4&         projViewMat,
    const glm::vec3&         cameraPos,
    const glm::mat4&         cullingProjViewMat,
    const glm::vec3&         cullingCameraPos
) {
}

} // namespace md