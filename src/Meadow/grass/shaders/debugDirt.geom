/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (line_strip, max_vertices = 2) out;
layout (location = 0) out vec4 o_color;

layout (points) in;
layout (location = 0) in vec3 i_normal[];

void main() {
    o_color = vec4(0.0, 1.0, 0.0, 1.0);
    gl_Position = p_projViewMat * gl_in[0].gl_Position;
    EmitVertex();
    o_color = vec4(0.0, 0.0, 1.0, 1.0);
    vec3 normal = normalize(i_normal[0]);
    gl_Position = p_projViewMat * (gl_in[0].gl_Position + vec4(normal, 0.0));
    EmitVertex();
}
