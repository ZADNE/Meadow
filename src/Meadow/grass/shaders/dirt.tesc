/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/grass/shaders/height.glsl>
#include <Meadow/grass/shaders/GrassPC.glsl>

layout (vertices = 4) out;
layout (location = 0) out vec2  o_pos[];
layout (location = 1) out ivec2 o_cull[];

layout (location = 0) in vec2 i_center[];

const vec2 k_edgeOffsets[] = vec2[](
    vec2(-4.0, +0.0),
    vec2(+0.0, -4.0),
    vec2(+4.0, +0.0),
    vec2(+0.0, +4.0)
);

void main() {
    // Control point position
    o_pos[gl_InvocationID] = i_center[0] + vec2(gl_InvocationID & 1, gl_InvocationID >> 1) * 8.0 - 4.0;

    // Outer tessellation level
    vec2 edge2D = i_center[0] + k_edgeOffsets[gl_InvocationID];
    float dist = distance(p_cameraPos.xyz, vec3(edge2D.x, height(edge2D, p_seed), edge2D.y));
    gl_TessLevelOuter[gl_InvocationID] = max(16.0 - dist * 0.0625, 1.0);

    // Frustum culling
    float h = height(o_pos[gl_InvocationID], p_seed);
    vec4 p = p_projViewMat * vec4(o_pos[gl_InvocationID].x, h, o_pos[gl_InvocationID].y, 1.0);
    p.w *= 1.25;
    o_cull[gl_InvocationID] = ivec2(int(p.x < -p.w) - int(p.x > p.w), int(p.y < -p.w) - int(p.y > p.w));
    barrier();
    uvec2 notIn = uvec2(notEqual(o_cull[0], ivec2(0, 0)));
    uvec2 eq01  = uvec2(equal(o_cull[0], o_cull[1]));
    uvec2 eq23  = uvec2(equal(o_cull[2], o_cull[3]));
    uvec2 eq02  = uvec2(equal(o_cull[0], o_cull[2]));
    bvec2 cull  = bvec2(eq01 * eq23 * eq02 * notIn);
    gl_TessLevelOuter[gl_InvocationID] = any(cull) ? 0.0 : gl_TessLevelOuter[gl_InvocationID];

    // Inner tessellation level
    if (gl_InvocationID < 2){
        gl_TessLevelInner[gl_InvocationID] =
            max(gl_TessLevelOuter[(gl_InvocationID) & 3], gl_TessLevelOuter[(gl_InvocationID + 2) & 3]);
    }
}
