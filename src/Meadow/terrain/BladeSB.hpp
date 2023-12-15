/*!
 *  @author     Dubsky Tomas
 */
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.hpp>

#include <Meadow/constants/map.hpp>

namespace md {

struct Blade {
    glm::vec4 posSize;
    glm::vec2 facing;
    glm::vec2 padding;
};

struct BladeSB {
    // Header
    vk::DrawIndirectCommand command;

    // Blades
    Blade blades[262143];
};

} // namespace md