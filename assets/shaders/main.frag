#version 460 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : requiere

layout (location = 0) in vec2 frag_tex_coord;
layout (location = 1) in vec3 frag_normal;

layout (location = 0) out vec4 res_color;

layout (set = 0, binding = 0) uniform sampler2D tex;

void main()
{
	res_color = texture(tex, frag_tex_coord) * vec4(frag_normal, 1.0f);
}