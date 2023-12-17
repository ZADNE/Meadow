/*!
 *  @author     Dubsky Tomas
 */
#ifndef CULLING_GLSL
#define CULLING_GLSL

/**
 * @brief Returns position of p relative to frustum planes 
 * @details -1 left, 0 in, +1 right for x component, analogous for y and z
 */
ivec3 frustumPlaneSigns(vec4 p){
    p.w *= 1.125;
    return ivec3(
        int(p.x < -p.w) - int(p.x > p.w),
        int(p.y < -p.w) - int(p.y > p.w),
                        - int(p.z > p.w));
}

/**
 * @brief Determines whether the line should be culled
 * @param p0, p1 Frustum planes of the points
 */
bool shouldBeCulled(ivec3 p0, ivec3 p1){
    uvec3 notIn = uvec3(notEqual(p0, ivec3(0)));
    uvec3 eq01  = uvec3(equal(p0, p1));
    bvec3 cull  = bvec3(eq01 * notIn);
    return any(cull);
}

/**
 * @brief Determines whether the rectangle should be culled
 * @param p0, p1, p2, p3 Frustum plane signs of the points
 */
bool shouldBeCulled(ivec3 p0, ivec3 p1, ivec3 p2, ivec3 p3){
    uvec3 notIn = uvec3(notEqual(p0, ivec3(0)));
    uvec3 eq01  = uvec3(equal(p0, p1));
    uvec3 eq23  = uvec3(equal(p2, p3));
    uvec3 eq02  = uvec3(equal(p0, p2));
    bvec3 cull  = bvec3(eq01 * eq23 * eq02 * notIn);
    return any(cull);
}

#endif // !CULLING_GLSL