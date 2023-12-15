/*!
 *  @author     Dubsky Tomas
 */
#ifndef HEIGHT_GLSL
#define HEIGHT_GLSL

#include <Meadow/terrain/external_shaders/snoise.glsl>

const float k_distNormalizer = 1.57079632679 / (512.0 * 512.0);

float groundHeight(vec2 pos, float seed){
    // Valley height
    float distSq = dot(pos, pos);
    vec2 dir = normalize(pos + 0.0001);
    float distortion = 1.0 + snoise(dir, seed) * 0.25;
    float valleyHeight = sin(distSq * distortion * k_distNormalizer) * -64.0;

    // Fractal-noise height
    float noiseHeight = 0.0;
    float s = 0.03125;
    for (int i = 0; i < 6; ++i){
        noiseHeight -= snoise(pos, seed) * s;
        pos *= 0.5;
        s *= 2.0;
    }

    return valleyHeight + noiseHeight;
}

const float k_derOffset = 0.0625;

vec3 groundNormal(vec2 pos, float posHeight, float seed){
    float h1 = groundHeight(pos + vec2(k_derOffset, 0.0), seed);
    float h2 = groundHeight(pos + vec2(0.0, k_derOffset), seed);
    vec3 v0 = vec3(-k_derOffset, posHeight - h1, 0.0);
    vec3 v1 = vec3(0.0,          posHeight - h2, -k_derOffset);
    return normalize(cross(v0, v1));
}

#endif // !HEIGHT_GLSL