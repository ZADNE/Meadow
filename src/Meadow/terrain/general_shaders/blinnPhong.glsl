/*!
 *  @author     Dubsky Tomas
 */
#ifndef BLINN_PHONG_GLSL
#define BLINN_PHONG_GLSL

/**
 * @brief Implements Blinn-Phong reflection model
 * @details All angles must be normalized!
 */
vec3 blinnPhong(vec3 albedo, vec3 normal, vec3 toLight, vec3 toViewer, float shininess, float specWeight){
    float diffuse = max(dot(normal, toLight), 0.0);

    vec3 halfway = normalize(toLight + toViewer);
    float specular = pow(max(dot(normal, halfway), 0.0), shininess);

    return albedo * diffuse + specular * specWeight;
}

#endif // !BLINN_PHONG_GLSL