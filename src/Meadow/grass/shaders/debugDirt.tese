/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/height.glsl>
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (location = 0) out vec3 o_normal;

layout (quads, fractional_even_spacing, point_mode) in;
layout (location = 0) in vec2 i_pos[];

const float k_derOffset = 0.0625;

void main() {
    // Calculate position within the patch
    vec2 pos01 = mix(i_pos[0], i_pos[1], gl_TessCoord.x);
    vec2 pos23 = mix(i_pos[2], i_pos[3], gl_TessCoord.x);
    vec2 pos = mix(pos01, pos23, gl_TessCoord.y);

    // Calculate height
    float h = height(pos, p_seed);
    gl_Position = vec4(pos.x, h, pos.y, 1.0);

    // Calculate normal
    float h1 = height(pos + vec2(k_derOffset, 0.0), p_seed);
    float h2 = height(pos + vec2(0.0, k_derOffset), p_seed);
    vec3 v0 = vec3(0.0, h, 0.0) - vec3(k_derOffset, h1, 0.0);
    vec3 v1 = vec3(0.0, h, 0.0) - vec3(0.0, h2, k_derOffset);
    o_normal = normalize(cross(v0, v1));
}
