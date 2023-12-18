/*!
 *  @author     Dubsky Tomas
 */
#version 460
#include <Meadow/generic/shaders/ViewFrustumPC.glsl>

layout (vertices = 4) out;
layout (location = 0) out vec3  o_pos[];

layout (location = 0) in  int   i_face[];


void main() {
    const int face = i_face[0];
    vec4 pos = p_viewFrustum.ndcToWorldMat * vec4(k_ndcCorners[face][gl_InvocationID], 1.0);

    o_pos[gl_InvocationID] = vec3(pos.xyz / pos.w);

    gl_TessLevelOuter[gl_InvocationID] = 31.0;
    if (gl_InvocationID < 2){
        gl_TessLevelInner[gl_InvocationID] = 31.0;
    }
}
