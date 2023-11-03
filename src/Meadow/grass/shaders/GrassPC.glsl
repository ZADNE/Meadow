/*!
 *  @author     Dubsky Tomas
 */
#ifndef GRASS_PC_GLSL
#define GRASS_PC_GLSL

#extension GL_EXT_scalar_block_layout : require
layout (push_constant, std430)
uniform GrassPC {
    mat4  p_projViewMat;
    vec4  p_cameraPos; // w unused
    float p_seed;
};

#endif // !GRASS_PC_GLSL