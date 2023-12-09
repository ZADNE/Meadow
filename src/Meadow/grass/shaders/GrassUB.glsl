/*!
 *  @author     Dubsky Tomas
 */
#ifndef GRASS_UB_GLSL
#define GRASS_UB_GLSL

#extension GL_EXT_scalar_block_layout : require
layout (set = 0, binding = GrassUB_BINDING, std430)
restrict uniform GrassUB {
    mat4  projViewMat;
    mat4  cullingProjViewMat;
    vec4  cameraPos;
    vec4  cullingCameraPos;
    vec2  windDir;
    float seed;
} u_grass;

#endif // !GRASS_UB_GLSL