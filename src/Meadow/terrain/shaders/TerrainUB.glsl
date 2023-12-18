/*!
 *  @author     Dubsky Tomas
 */
#ifndef GRASS_UB_GLSL
#define GRASS_UB_GLSL

#extension GL_EXT_scalar_block_layout : require
layout (set = 0, binding = TerrainUB_BINDING, std430)
restrict uniform TerrainUB {
    mat4  projViewMat;
    mat4  cullingProjViewMat;
    vec4  cameraPos;
    vec4  cullingCameraPos;
    vec2  windOffset;
    float seed;
    float timeSec;
    uint  maxBladesPerTile;
} u_terrain;

#endif // !GRASS_UB_GLSL