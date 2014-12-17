#version 430
// Vertex program
in vec3  position;
in float color;
in vec3  normal;

layout(location = 0) uniform mat4 model_matrix;
layout(location = 1) uniform mat4 shadow_matrix;
layout(location = 2) uniform vec3  clip_normal;
layout(location = 3) uniform float clip_distance;

layout(location = 0) out vec4 out_position;

void main() {
	gl_Position = shadow_matrix * model_matrix * vec4(position, 1.0);
	out_position = gl_Position;

    gl_ClipDistance[0] = -dot(position.xyz, clip_normal) + clip_distance;
}
