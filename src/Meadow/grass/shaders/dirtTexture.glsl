/*!
 *  @author     Dubsky Tomas
 */
#ifndef DIRT_TEXTURE_GLSL
#define DIRT_TEXTURE_GLSL

#include <Meadow/grass/external_shaders/snoise.glsl>

const float k_distNormalizer = 1.57079632679 / (512.0 * 512.0);

vec3 dirtTexture(vec2 pos, float seed){
    return vec3(1.0, 0.0, 0.0);
}

#endif // !DIRT_TEXTURE_GLSL