/*!
 *  @author     Dubsky Tomas
 */
#version 460
const uint GrassUB_BINDING = 0;
#include <Meadow/grass/shaders/GrassUB.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec4   i_posSize;
layout (location = 1) in vec2   i_facing;

const vec2 k_stalkOffsets[] = vec2[](
    vec2(0.04,      0.0),
    vec2(0.038205,  0.2),
    vec2(0.034497,  0.4),
    vec2(0.029043,  0.6),
    vec2(0.022815,  0.8),
    vec2(0.016139,  0.9),
    vec2(0.0,       1.0)
);

void main() {
    const float size = i_posSize.w;

    const vec2 sizeScale = vec2(1.0 + (size - 1.0) * 0.5, size);
    const vec2 stalkOffset = k_stalkOffsets[gl_VertexIndex >> 1];
    const vec2 windDir = u_grass.windDir;
    const vec2 offset = i_facing * stalkOffset.x * (float(gl_VertexIndex & 1) * 2.0 - 1.0);

    // Control points
    const vec3  cp0 = i_posSize.xyz; // Root
    const vec3  cp1 = cp0 + vec3(0.0, sizeScale.y * -0.5, 0.0); // Control
    const vec3  cp2 = cp0 - sizeScale.y; // Tip

    // Helper vars
    const float t = stalkOffset.y;
    const float ti = 1.0 - t;
    const vec3 p2_1 = cp2 - cp1;

    // Bezier curve
    vec3 pos = cp1 + ti * ti * (cp0 - cp1) + t * t * p2_1;

    pos.xz += offset;
    gl_Position = u_grass.projViewMat * vec4(pos, 1.0);
    o_color = vec4(0.0, 1.0, 0.0, 1.0);
}
