/*!
 *  @author     Dubsky Tomas
 */
#ifndef STALK_SB_GLSL
#define STALK_SB_GLSL
#include <Meadow/constants/terrain.glsl>

struct IndirectCommand {
    uint    vertexCount;
    uint    instanceCount;
    uint    firstVertex;
    uint    firstInstance;
};

struct Blade {
    float   x, y, z;    // root position; broken down to avoid alignment requirements
    float   sway;       // sway strength
    uint    tiltDir;    // packed vec2; x = horizontal, y = vertical
    uint    facingDir;  // packed vec2; x = x, y = z
    float   size;       // Length of the blade
    uint    hashAndLOD; // packed vec2; x = random hash in range 0..1, y = LOD factor in range 0..1
    uint    rootTint;   // packed vec4
    uint    tipTint;    // packed vec4
};

layout (set = 0, binding = BladeSB_BINDING, std430)
restrict buffer BladeSB {
    // Header
    IndirectCommand command;

    // Blades
    Blade blades[1048575];
} b_blade;

#endif // !STALK_SB_GLSL