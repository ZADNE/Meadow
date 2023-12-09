/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/height.glsl>
const uint GrassUB_BINDING = 0;
#include <Meadow/grass/shaders/GrassUB.glsl>


layout (location = 0) out vec3 o_normal;

layout (quads, fractional_even_spacing, point_mode) in;
layout (location = 0) in vec2 i_pos2D[];

const float k_derOffset = 0.0625;

void main() {
    // Calculate position within the patch
    vec2 pos = i_pos2D[0] + gl_TessCoord.xy * 8.0;

    // Calculate height
    float h = height(pos, u_grass.seed);
    gl_Position = vec4(vec3(pos.x, h, pos.y), 1.0);

    // Calculate normal
    float h1 = height(pos + vec2(k_derOffset, 0.0), u_grass.seed);
    float h2 = height(pos + vec2(0.0, k_derOffset), u_grass.seed);
    vec3 v0 = vec3(-k_derOffset, h - h1, 0.0);
    vec3 v1 = vec3(0.0,          h - h2, -k_derOffset);
    o_normal = normalize(cross(v0, v1));
}
