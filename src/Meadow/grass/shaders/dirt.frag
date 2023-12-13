/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/general_shaders/blinnPhong.glsl>
#include <Meadow/constants/environment.glsl>
#include <Meadow/grass/shaders/dirtTexture.glsl>
const uint GrassUB_BINDING = 0;
#include <Meadow/grass/shaders/GrassUB.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

void main() {
    vec3 normal = normalize(i_normal);
    vec2 pos2D = i_pos.xz;

    vec4 albedoShine = dirtTexture(pos2D, normal, u_grass.seed);
    vec3 bump = dirtBump(pos2D, normal, gl_FragCoord.z, u_grass.seed);
    vec3 toView = normalize(u_grass.cameraPos.xyz - i_pos);

    vec3 color = blinnPhong(albedoShine.rgb, bump, k_toSun, toView, albedoShine.w, 0.0625);

    o_color = vec4(color, 1.0);
}
