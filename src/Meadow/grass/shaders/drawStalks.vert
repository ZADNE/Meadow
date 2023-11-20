/*!
 *  @author     Dubsky Tomas
 */
#version 460
const uint StalkSB_BINDING = 0;
#include <Meadow/grass/shaders/StalkSB.glsl>
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec4   i_posSize;
layout (location = 1) in vec2   i_facing;

const vec2 k_stalkOffsets[] = vec2[](
    vec2(0.04,      -0.0),
    vec2(0.038205,  -0.2),
    vec2(0.034497,  -0.4),
    vec2(0.029043,  -0.6),
    vec2(0.022815,  -0.8),
    vec2(0.016139,  -0.9),
    vec2(0.0,       -1.0)
);

void main() {
    vec3 pos   = i_posSize.xyz;
    float size = i_posSize.w;

    vec2 sizeScale = vec2(1.0 + (size - 1.0) * 0.5, size);
    vec2 offset = k_stalkOffsets[gl_VertexIndex >> 1] * sizeScale;
    offset.x *= float(gl_VertexIndex & 1) * 2.0 - 1.0;
    pos.xy += offset;
    gl_Position = p_grass.projViewMat * vec4(pos, 1.0);
    o_color = vec4(0.0, 1.0, 0.0, 1.0);
}
