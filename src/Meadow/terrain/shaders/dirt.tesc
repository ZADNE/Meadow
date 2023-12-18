/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/generic/shaders/culling.glsl>
#include <Meadow/terrain/shaders/ground.glsl>
const uint TerrainUB_BINDING = 0;
#include <Meadow/terrain/shaders/TerrainUB.glsl>

layout (vertices = 4) out;
layout (location = 0) out vec2  o_pos2D[];
layout (location = 1) out ivec3 o_cull[];

layout (location = 0) in vec2 i_center[];

const vec2 k_edgeOffsets[] = vec2[](
    vec2(-4.0, +0.0),
    vec2(+0.0, -4.0),
    vec2(+4.0, +0.0),
    vec2(+0.0, +4.0)
);

void main() {
    // Control point position
    o_pos2D[gl_InvocationID] = i_center[0] + vec2(gl_InvocationID & 1, gl_InvocationID >> 1) * 8.0 - 4.0;

    // Outer tessellation level
    vec2 edge2D = i_center[0] + k_edgeOffsets[gl_InvocationID];
    float dist = distance(
                    u_terrain.cullingCameraPos.xyz,
                    vec3(edge2D.x, groundHeight(edge2D, u_terrain.seed), edge2D.y));
    gl_TessLevelOuter[gl_InvocationID] = max(16.0 - dist * 0.0625, 1.0);

    // Test whether each point is inside frustum
    float h = groundHeight(o_pos2D[gl_InvocationID], u_terrain.seed);
    o_cull[gl_InvocationID] = frustumPlaneSigns(
        u_terrain.cullingProjViewMat * vec4(o_pos2D[gl_InvocationID].x, h, o_pos2D[gl_InvocationID].y, 1.0));
    barrier();

    // Frustum cull if all points are outside frustum
    bool cull = shouldBeCulled(o_cull[0], o_cull[1], o_cull[2], o_cull[3]);
    gl_TessLevelOuter[gl_InvocationID] = cull ? 0.0 : gl_TessLevelOuter[gl_InvocationID];

    // Inner tessellation level
    if (gl_InvocationID < 2){
        gl_TessLevelInner[gl_InvocationID] =
            max(gl_TessLevelOuter[(gl_InvocationID) & 3], gl_TessLevelOuter[(gl_InvocationID + 2) & 3]);
    }
}
