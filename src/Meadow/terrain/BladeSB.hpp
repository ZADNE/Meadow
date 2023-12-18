/*!
 *  @author     Dubsky Tomas
 */
#pragma once
#include <glm/vec4.hpp>
#include <vulkan/vulkan.hpp>

#include <Meadow/constants/terrain.hpp>

namespace md {

struct Blade {
    glm::vec4 posAndSway; // xyz = root position, w = sway strength
    glm::uint tiltDir;    // packed vec2; x = horizontal, y = vertical
    glm::uint facingDir;  // packed vec2; x = x, y = z
    float     size;
    glm::uint hashAndLOD; // packed vec2; x = random hash in range 0..1,
                          // y = LOD factor in range 0..1
};

struct BladeSB {
    // Header
    vk::DrawIndirectCommand command;

    // Blades
    Blade blades[1048575];
};

} // namespace md