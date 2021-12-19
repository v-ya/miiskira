#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
	vec4 gl_Position;
};

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_color;

layout (location = 0) out vec4 frag_color;

layout (binding = 0) uniform uf {
	mat4 viewport;
} u;

void main(void)
{
	gl_Position = u.viewport * vec4(vert_pos, 1.0);
	frag_color = vec4(vert_color, 1.0);
}
