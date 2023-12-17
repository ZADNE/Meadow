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
    glm::vec4 posAndhash; // xyz = root position, w = random hash in range 0..1
    glm::vec2 tipOffset;  // x = horizontal, y = vertical
    glm::uint facingDir;  // packed vec2
    float     swayStrength;
};

struct BladeSB {
    // Header
    vk::DrawIndirectCommand command;

    // Blades
    Blade blades[262143];
};

} // namespace md