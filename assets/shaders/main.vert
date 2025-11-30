#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout (location = 0) out vec2 frag_tex_coord;
layout (location = 1) out vec3 frag_normal;

layout (set = 0, binding = 1) uniform UniformTransformMatrices
{
	mat4 MVP;
} utm;

void main()
{
	gl_Position = utm.MVP * vec4(aPos, 1.0f);
	frag_tex_coord = aTexCoord;
	frag_normal = aNormal;
}