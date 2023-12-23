/*!
 *  @author     Dubsky Tomas
 */
#pragma once
#include <glm/vec4.hpp>
#include <vulkan/vulkan.hpp>

#include <Meadow/constants/terrain.hpp>

namespace md {

struct Blade {
    float     x, y, z;    // root position; broken down to avoid alignment reqmt
    float     sway;       // sway strength
    glm::uint tiltDir;    // snorm vec2; x = horizontal, y = vertical
    glm::uint facingDir;  // snorm vec2; x = x, y = z
    float     size;       // Length of the blade
    glm::uint hashAndLOD; // unorm vec2; x = random hash, y = LOD factor
    glm::uint rootTint;   // unorm vec4
    glm::uint tipTint;    // unorm vec4
};

struct BladeSB {
    // Header
    vk::DrawIndirectCommand command;

    // Blades
    Blade blades[1048575];
};

} // namespace md