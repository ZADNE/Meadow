/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/generic/shaders/ViewFrustumPC.glsl>

layout (location = 0) out vec4 o_color;

layout (quads, equal_spacing) in;
layout (location = 0) in vec3 i_pos[];


void main() {
    vec3 pos01 = mix(i_pos[0], i_pos[1], gl_TessCoord.x);
    vec3 pos23 = mix(i_pos[2], i_pos[3], gl_TessCoord.x);
    vec3 pos   = mix(pos01,    pos23,    gl_TessCoord.y);

    gl_Position = p_viewFrustum.projViewMat * vec4(pos, 1.0);
    o_color = vec4(1.0, 1.0, 1.0, 1.0);
}
