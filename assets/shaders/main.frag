#version 460 core

layout (location = 0) in vec2 frag_tex_coord;
layout (location = 1) in vec3 frag_normal;

layout (location = 0) out vec4 res_color;

layout (set = 0, binding = 0) uniform sampler2D tex;

void main()
{
	res_color = vec4(texture(tex, frag_tex_coord).xyz * frag_normal, 1.0f);
}