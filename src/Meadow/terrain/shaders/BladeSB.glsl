/*!
 *  @author     Dubsky Tomas
 */
#ifndef STALK_SB_GLSL
#define STALK_SB_GLSL
#include <Meadow/constants/map.glsl>

struct IndirectCommand {
    uint    vertexCount;
    uint    instanceCount;
    uint    firstVertex;
    uint    firstInstance;
};

struct Blade {
    vec4    posAndhash; // xyz = root position, w = random hash in range 0..1
    vec2    tipOffset; // x = horizontal, y = vertical
    uint    facingDir; // packed vec2
    float   swayStrength;
};

layout (set = 0, binding = BladeSB_BINDING, std430)
restrict buffer BladeSB {
    // Header
    IndirectCommand command;

    // Blades
    Blade blades[262143];
} b_blade;

#endif // !STALK_SB_GLSL