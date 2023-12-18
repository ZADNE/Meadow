/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/terrain/external_shaders/blinnPhong.glsl>
#include <Meadow/constants/environment.glsl>
const uint TerrainUB_BINDING = 0;
#include <Meadow/terrain/shaders/TerrainUB.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec3  i_pos;
layout (location = 1) in vec3  i_normal;
layout (location = 2) in vec3  i_albedo;
layout (location = 3) in float i_shininess;

void main() {
    vec3 normal = normalize(i_normal);

    vec3 toView = normalize(u_terrain.cameraPos.xyz - i_pos);

    vec3 color = blinnPhong(i_albedo, normal, k_toSun, toView, i_shininess, vec2(0.5, 0.375));

    o_color = vec4(color, 1.0);
}
