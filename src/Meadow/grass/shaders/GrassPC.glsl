/*!
 *  @author     Dubsky Tomas
 */
#ifndef GRASS_PC_GLSL
#define GRASS_PC_GLSL

#extension GL_EXT_scalar_block_layout : require
layout (push_constant, std430)
uniform GrassPC {
    mat4  projViewMat;
    vec4  cameraPos; // w unused
    vec2  windDir;
    float seed;
} p_grass;

#endif // !GRASS_PC_GLSL