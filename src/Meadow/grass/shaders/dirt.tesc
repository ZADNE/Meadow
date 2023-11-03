/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/height.glsl>
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (vertices = 4) out;
layout (location = 0) out vec2 o_pos[];

layout (location = 0) in vec2 i_center[];

const vec2 k_sideOffsets[] = vec2[](
    vec2(-4.0, +0.0),
    vec2(+0.0, -4.0),
    vec2(+4.0, +0.0),
    vec2(+0.0, +4.0)
);

void main() {
    // Control point position
    o_pos[gl_InvocationID] = i_center[0] + vec2(gl_InvocationID & 1, gl_InvocationID >> 1) * 8.0 - 4.0;

    // Outer tessellation level
    vec2 side2D = i_center[0] + k_sideOffsets[gl_InvocationID];
    float dist = distance(-p_cameraPos.xyz, vec3(side2D.x, height(side2D, p_seed), side2D.y));
    gl_TessLevelOuter[gl_InvocationID] = max(16.0 - dist * 0.0625, 1.0);

    // Inner tessellation level
    barrier();
    if (gl_InvocationID < 2){
        gl_TessLevelInner[gl_InvocationID] =
            max(gl_TessLevelOuter[(gl_InvocationID) & 3], gl_TessLevelOuter[(gl_InvocationID + 2) & 3]);
    }
}
