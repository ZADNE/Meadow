/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/constants/terrain.glsl>

layout (location = 0) out vec2 o_center;

void main() {
    o_center = (vec2(indexToGridPos(gl_VertexIndex)) + 0.5) * k_dirtTileSize;
}
