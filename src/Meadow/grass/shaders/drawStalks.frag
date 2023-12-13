/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/general_shaders/blinnPhong.glsl>
#include <Meadow/constants/environment.glsl>
const uint GrassUB_BINDING = 0;
#include <Meadow/grass/shaders/GrassUB.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec3 i_albedo;

void main() {
    vec3 normal = normalize(i_normal);

    float shininess = 80.0;

    vec3 toView = normalize(u_grass.cameraPos.xyz - i_pos);

    vec3 color = blinnPhong(i_albedo, normal, k_toSun, toView, shininess, 0.0625);

    o_color = vec4(color, 1.0);
}
