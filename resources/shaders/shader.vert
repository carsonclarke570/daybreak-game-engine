#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform MMat {
    mat4 model;
} m_mat;

layout(binding = 1) uniform VPMat {
    mat4 view;
    mat4 proj;
} vp_mat;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;
layout(location = 3) in vec2 in_texcoord;

layout(location = 0) out vec3 frag_color;

void main() {
    gl_Position = vp_mat.proj * vp_mat.view * m_mat.model * vec4(in_position, 1.0);
    frag_color = in_color;
}