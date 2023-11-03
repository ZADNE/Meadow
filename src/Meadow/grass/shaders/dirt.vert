/*!
 *  @author     Dubsky Tomas
 */
#version 460
layout (location = 0) out vec2 o_center;

void main() {
    o_center = vec2((gl_VertexIndex & 127) - 64, (gl_VertexIndex >> 7) - 64) * 8.0 + 4.0;
}
