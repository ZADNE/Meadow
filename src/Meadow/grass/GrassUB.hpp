/*!
 *  @author     Dubsky Tomas
 */
#pragma once
#include <glm/mat4x4.hpp>

namespace md {

struct GrassUB {
    glm::mat4 projViewMat;
    glm::mat4 cullingProjViewMat;
    glm::vec4 cameraPos;        // w unused
    glm::vec4 cullingCameraPos; // w unused
    glm::vec2 windDir;
    float     seed;
};

} // namespace md