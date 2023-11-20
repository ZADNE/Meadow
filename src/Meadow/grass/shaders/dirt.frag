/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/dirtTexture.glsl>
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

const vec3 k_toSun = vec3(0.259160548, -0.863868415, 0.431934208);

void main() {
    vec3 normal = normalize(i_normal);
    vec2 pos2D = i_pos.xz;
    vec4 color_shine = dirtTexture(pos2D, normal, p_grass.seed);
    vec3 bump = dirtBump(pos2D, normal, gl_FragCoord.z, p_grass.seed);

    float diffuse = max(dot(bump, k_toSun), 0.0);

    vec3 view = normalize(p_grass.cameraPos.xyz - i_pos);
    vec3 r = reflect(-k_toSun, bump);
    float specular = pow(max(dot(view, r), 0.0), color_shine.w);

    o_color = vec4(color_shine.rgb * diffuse + specular * 0.0625, 1.0);
}
