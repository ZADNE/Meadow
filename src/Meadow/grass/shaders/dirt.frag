/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/dirtTexture.glsl>
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec3 i_normal;
layout (location = 1) in vec2 i_pos2D;

const vec3 k_sunDir = vec3(0.0990147516, -0.990147531, 0.0990147516);

void main() {
    vec3 color = dirtTexture(i_pos2D, p_seed);

    vec3 normal = normalize(i_normal);
    float lambertian = max(dot(normal, k_sunDir), 0.0);

    o_color = vec4(color * lambertian, 1.0);
}
