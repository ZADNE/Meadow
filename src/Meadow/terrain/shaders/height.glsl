/*!
 *  @author     Dubsky Tomas
 */
#ifndef HEIGHT_GLSL
#define HEIGHT_GLSL

#include <Meadow/terrain/external_shaders/snoise.glsl>

const float k_distNormalizer = 1.57079632679 / (512.0 * 512.0);

float height(vec2 pos, float seed){
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

#endif // !HEIGHT_GLSL