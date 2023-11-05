/*!
 *  @author     Dubsky Tomas
 */
#version 460
layout (location = 0) out vec4 o_color;

void main() {
    uint id = gl_VertexIndex + gl_DrawID;
    gl_Position = vec4(id & 1, id >> 1, 0.0, 1.0);
    o_color = vec4(gl_DrawID, 0.0, 0.0, 1.0);
}
