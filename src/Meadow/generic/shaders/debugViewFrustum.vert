/*!
 *  @author     Dubsky Tomas
 */
#version 460

layout (location = 0) out int o_face;

void main(){
    o_face = gl_InstanceIndex;
}
