/*!
 *  @author     Dubsky Tomas
 */
#ifndef DIRT_TEXTURE_GLSL
#define DIRT_TEXTURE_GLSL

#include <Meadow/terrain/external_shaders/snoise.glsl>

const float k_distNormalizer = 1.57079632679 / (512.0 * 512.0);

const vec3 k_wetRich  = vec3(0.32, 0.24, 0.21);
const vec3 k_dryRich  = vec3(0.38, 0.28, 0.20);
const vec3 k_wetSandy = vec3(0.42, 0.34, 0.24);
const vec3 k_drySandy = vec3(0.79, 0.59, 0.39);

// rgb = albedo, w = shininess
vec4 dirtTexture(vec2 pos2D, vec3 normal, float seed){
    float richness = 0.5;
    float wetness = 0.5;
    {
        vec2 p = pos2D * 0.0625 + vec2(7759.0, 7699.0);
        float s = 0.03125;
        for (int i = 0; i < 6; ++i){
            richness += snoise(p, seed) * s;
            wetness += snoise(p, seed + 7103.0) * s;
            p *= 0.5;
            s *= 2.0;
        }
    }
    wetness = (wetness - normal.y) * 0.5;

    vec3 rich = mix(k_dryRich, k_wetRich, wetness);
    vec3 sandy = mix(k_drySandy, k_wetSandy, wetness);
    vec4 rval;
    rval.rgb = mix(sandy, rich, richness);
    rval.w = mix(5.0, 40.0, wetness);
    return rval;
}

// xyz = modified normal
vec3 dirtBump(vec2 pos2D, vec3 normal, float d, float seed){
    // Weaken the effect at far to avoid noise
    float f = clamp((0.99 - gl_FragCoord.z) * 50.0, 0.25, 0.75);
    float bumpY = normal.y + snoise(pos2D * 64.0, seed) * f;
    return normalize(vec3(normal.x, bumpY, normal.z));
}

#endif // !DIRT_TEXTURE_GLSL