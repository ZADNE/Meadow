/*!
 *  @author     Dubsky Tomas
 */
#version 460
const uint TerrainUB_BINDING = 0;
#include <Meadow/terrain/shaders/TerrainUB.glsl>

layout (location = 0) out vec3  o_pos;
layout (location = 1) out vec3  o_normal;
layout (location = 2) out vec3  o_albedo;
layout (location = 3) out float o_shininess;

layout (location = 0) in vec4  i_posAndSway;
layout (location = 1) in vec2  i_tiltDir;
layout (location = 2) in vec2  i_facing;
layout (location = 3) in float i_size;
layout (location = 4) in vec2  i_hashAndLOD;

const vec2 k_bladeOffsets[] = vec2[](
    vec2(0.04,      0.00001),
    vec2(0.038205,  0.2),
    vec2(0.034497,  0.4),
    vec2(0.029043,  0.6),
    vec2(0.022815,  0.8),
    vec2(0.016139,  0.9),
    vec2(0.0,       1.0)
);

void main() {
    const float hash = i_hashAndLOD.x;
    const float lod  = i_hashAndLOD.y;
    const float swayStrength = i_posAndSway.w;
    const float width = i_size + (1.0 - lod) * 8.0;
    const float length = i_size + (1.0 - lod);

    const vec2 vertexOffset = k_bladeOffsets[gl_VertexIndex >> 1];

    // Control points
    const vec3  cp0 = i_posAndSway.xyz; // Root
    const vec3  cp1 = cp0 + vec3(0.0, i_tiltDir.y * -0.5 * length, 0.0); // Control
    const vec3  cp2 = cp0 + vec3(i_tiltDir.x * i_facing.x, -i_tiltDir.y, i_tiltDir.x * i_facing.y) * length; // Tip

    // Bezier helper vars
    const float t = vertexOffset.y;
    const float ti = 1.0 - t;
    const vec3 p2_1 = cp2 - cp1;

    // Bezier curve
    vec3 pos = cp1 + ti * ti * (cp0 - cp1) + t * t * p2_1;

    // Curve derivative
    vec3 der = ti * (cp1 - cp0) + t * p2_1;

    // Principal directions
    const float sideSign = (float(gl_VertexIndex & 1) * 2.0 - 1.0);
    vec3 dir = normalize(der);
    vec3 sideDir = vec3(-i_facing.y, 0.0, i_facing.x);
    vec3 normal = cross(dir, sideDir);

    { // Wave sway
        float wavePhase = hash * 6.28318530718
                        + u_terrain.timeSec * (12.0 + hash * 8.0)
                        + t * 4.0;
        float waveStrength = t * t * swayStrength * swayStrength * lod * lod;
        pos += normal * sin(wavePhase) * waveStrength;
        pos += sideDir * sin(wavePhase * 0.75) * waveStrength * 0.25;
    }

    // Offset to side from axis of the curve
    pos.xz += sideDir.xz * vertexOffset.x * sideSign * width;

    // Output
    gl_Position = u_terrain.projViewMat * vec4(pos, 1.0);
    o_pos = pos;
    o_normal = normalize( // Tilt normal outwards
                normal * 0.75 +
                sideDir * 0.25 * sideSign * sign(vertexOffset.x) * lod);
    o_albedo = vec3(0.286, 0.376, 0.106);
    o_shininess = 40.0 + lod * 40.0;
}
