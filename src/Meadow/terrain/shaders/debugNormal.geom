/*!
 *  @author     Dubsky Tomas
 */
#version 460
const uint TerrainUB_BINDING = 0;
#include <Meadow/terrain/shaders/TerrainUB.glsl>

layout (line_strip, max_vertices = 2) out;
layout (location = 0) out vec4 o_color;

layout (points) in;
layout (location = 0) in vec3 i_pos[];
layout (location = 1) in vec3 i_normal[];

void main() {
    o_color = vec4(1.0, 0.0, 0.0, 1.0);
    gl_Position = u_terrain.projViewMat * vec4(i_pos[0], 1.0);
    EmitVertex();
    o_color = vec4(0.0, 0.0, 1.0, 1.0);
    gl_Position = u_terrain.projViewMat * (vec4(i_pos[0] + i_normal[0], 1.0));
    EmitVertex();
}
