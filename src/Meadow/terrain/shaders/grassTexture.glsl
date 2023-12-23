/*!
 *  @author     Dubsky Tomas
 */
#ifndef GRASS_TEXTURE_GLSL
#define GRASS_TEXTURE_GLSL

#include <Meadow/terrain/shaders/ground.glsl>

const vec3 k_wetRichGrass  = vec3(0.239, 0.349, 0.176);
const vec3 k_dryRichGrass  = vec3(0.286, 0.375, 0.106);
const vec3 k_wetSandyGrass = vec3(0.592, 0.569, 0.047);
const vec3 k_drySandyGrass = vec3(0.384, 0.424, 0.075);

// rgb = albedo, w = shininess
vec4 grassTexture(vec2 richWet){
    vec3 rich = mix(k_dryRichGrass, k_wetRichGrass, richWet.y);
    vec3 sandy = mix(k_drySandyGrass, k_wetSandyGrass, richWet.y);
    vec4 rval;
    rval.rgb = mix(sandy, rich, richWet.x);
    rval.w = mix(5.0, 40.0, richWet.y);
    return rval;
}

#endif // !GRASS_TEXTURE_GLSL