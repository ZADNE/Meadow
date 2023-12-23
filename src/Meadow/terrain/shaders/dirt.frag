/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/generic/external_shaders/blinnPhong.glsl>
#include <Meadow/constants/environment.glsl>
#include <Meadow/terrain/shaders/dirtTexture.glsl>
const uint TerrainUB_BINDING = 0;
#include <Meadow/terrain/shaders/TerrainUB.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

void main() {
    vec3 normal = normalize(i_normal);
    vec2 pos2D = i_pos.xz;

    vec2 richWet = groundRichnessAndWetness(pos2D, u_terrain.seed);

    vec4 albedoShine = dirtTexture(richWet, normal);
    vec3 bump = dirtBump(pos2D, normal, gl_FragCoord.z, u_terrain.seed);
    vec3 toView = normalize(u_terrain.cameraPos.xyz - i_pos);

    vec3 color = blinnPhong(albedoShine.rgb, bump, k_toSun, toView, albedoShine.w, vec2(0.0, 0.0625));

    o_color = vec4(color, 1.0);
}
