/*!
 *  @author     Dubsky Tomas
 */
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.hpp>

#include <Meadow/constants/map.hpp>

namespace md {

struct Stalk {
    glm::vec4 posSize;
    glm::vec2 facing;
    glm::vec2 padding;
};

#pragma warning(push)
#pragma warning(disable : 4200)
struct StalkSB {
    // Header
    vk::DrawIndirectCommand command;

    // Stalks
    Stalk stalks[];
};
#pragma warning(pop)

} // namespace md