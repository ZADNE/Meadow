/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/terrain/shaders/ground.glsl>
const uint TerrainUB_BINDING = 0;
#include <Meadow/terrain/shaders/TerrainUB.glsl>

layout (location = 0) out vec3 o_pos;
layout (location = 1) out vec3 o_normal;

layout (quads, fractional_even_spacing) in;
layout (location = 0) in vec2 i_pos2D[];

void main() {
    // Calculate global 2D position
    vec2 pos = i_pos2D[0] + gl_TessCoord.xy * 8.0;

    // Calculate height
    float h = groundHeight(pos, u_terrain.seed);
    o_pos = vec3(pos.x, h, pos.y);
    gl_Position = u_terrain.projViewMat * vec4(o_pos, 1.0);

    // Calculate normal
    o_normal = groundNormal(pos, h, u_terrain.seed);
}
