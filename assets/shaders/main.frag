#version 460 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec4 frag_color;
layout (location = 1) in vec2 frag_tex_coord;

layout (location = 0) out vec4 res_color;

layout (set = 0, binding = 0) uniform sampler2D tex;

void main()
{
	res_color = texture(tex, frag_tex_coord) * frag_color;
}