/*!
 *  @author     Dubsky Tomas
 */
#ifndef VIEW_FRUSTUM_PC_GLSL
#define VIEW_FRUSTUM_PC_GLSL

layout (push_constant, std430)
restrict uniform ViewFrustumPC {
    mat4 projViewMat;
    mat4 ndcToWorldMat;
} p_viewFrustum;

const float far = 0.9999;

const vec3[5][4] k_ndcCorners = vec3[5][4](
    vec3[4](vec3(-1.f, -1.f, far), vec3(+1.f, -1.f, far), vec3(-1.f, +1.f, far), vec3(+1.f, +1.f, far)),
    vec3[4](vec3(-1.f, -1.f, 0.f), vec3(-1.f, -1.f, far), vec3(-1.f, +1.f, 0.f), vec3(-1.f, +1.f, far)),
    vec3[4](vec3(+1.f, -1.f, 0.f), vec3(+1.f, -1.f, far), vec3(+1.f, +1.f, 0.f), vec3(+1.f, +1.f, far)),
    vec3[4](vec3(-1.f, +1.f, 0.f), vec3(+1.f, +1.f, 0.f), vec3(-1.f, +1.f, far), vec3(+1.f, +1.f, far)),
    vec3[4](vec3(-1.f, -1.f, 0.f), vec3(+1.f, -1.f, 0.f), vec3(-1.f, -1.f, far), vec3(+1.f, -1.f, far))
);

#endif // !VIEW_FRUSTUM_PC_GLSL