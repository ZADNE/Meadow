/*!
 *  @author     Dubsky Tomas
 */
#ifndef DIRT_TEXTURE_GLSL
#define DIRT_TEXTURE_GLSL

#include <Meadow/terrain/shaders/ground.glsl>

const vec3 k_wetRichDirt  = vec3(0.32, 0.24, 0.21);
const vec3 k_dryRichDirt  = vec3(0.38, 0.28, 0.20);
const vec3 k_wetSandyDirt = vec3(0.42, 0.34, 0.24);
const vec3 k_drySandyDirt = vec3(0.79, 0.59, 0.39);

// rgb = albedo, w = shininess
vec4 dirtTexture(vec2 richWet, vec3 normal){
    richWet.y = (richWet.y - normal.y) * 0.5;

    vec3 rich = mix(k_dryRichDirt, k_wetRichDirt, richWet.y);
    vec3 sandy = mix(k_drySandyDirt, k_wetSandyDirt, richWet.y);
    vec4 rval;
    rval.rgb = mix(sandy, rich, richWet.x);
    rval.w = mix(5.0, 40.0, richWet.y);
    return rval;
}

// xyz = modified normal
vec3 dirtBump(vec2 pos2D, vec3 normal, float d, float seed){
    // Weaken the effect at far to avoid noise
    float f = clamp((0.99 - d) * 50.0, 0.25, 0.75);
    float bumpY = normal.y + snoise(pos2D * 64.0, seed) * f;
    return normalize(vec3(normal.x, bumpY, normal.z));
}

#endif // !DIRT_TEXTURE_GLSL