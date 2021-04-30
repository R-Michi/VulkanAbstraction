#version 460 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec2 frag_tex_coord;

layout (set = 0, binding = 1) uniform UniformTransformMatrices
{
	mat4 MVP;
} utm;

void main()
{
	gl_Position = utm.MVP * vec4(aPos, 1.0f);
	frag_color = aColor;
	frag_tex_coord = aTexCoord;
}