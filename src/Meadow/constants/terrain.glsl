/*!
 *  @author     Dubsky Tomas
 */
#ifndef TERRAIN_GLSL
#define TERRAIN_GLSL

const ivec2 k_dirtTileSize       = ivec2(8, 8);

const ivec2 k_dirtGridSize       = ivec2(64, 64);
const ivec2 k_dirtGridMask       = k_dirtGridSize - 1;
const ivec2 k_dirtGridHalfSize   = k_dirtGridSize / 2;
const ivec2 k_dirtGridBits       = ivec2(6, 6);

ivec2 indexToGridPos(int index){
    return ivec2(index & k_dirtGridMask.x, index >> k_dirtGridBits.y) - k_dirtGridHalfSize;
}

#endif // !TERRAIN_GLSL