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

struct Stalk {
    vec4    posSize;
    vec2    facing;
    vec2    padding;
};

layout (set = 0, binding = StalkSB_BINDING, std430)
restrict buffer StalkSB {
    // Header
    IndirectCommand command;

    // Stalks
    Stalk   stalks[262143];
} b_stalk;

#endif // !STALK_SB_GLSL