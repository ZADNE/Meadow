/*!
 *  @author     Dubsky Tomas
 */
#ifndef MAP_GLSL
#define MAP_GLSL

const ivec2 k_mapTileSize       = ivec2(8, 8);

const ivec2 k_mapGridSize       = ivec2(128, 128);
const ivec2 k_mapGridMask       = k_mapGridSize - 1;
const ivec2 k_mapGridHalfSize   = k_mapGridSize / 2;
const ivec2 k_mapGridBits       = ivec2(7, 7);

ivec2 indexToGridPos(int index){
    return ivec2(index & k_mapGridMask.x, index >> k_mapGridBits.y) - k_mapGridHalfSize;
}

#endif // !MAP_GLSL