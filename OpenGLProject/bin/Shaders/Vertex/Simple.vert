#version 410

layout (location = 0) in vec4 Position;

uniform mat4 ProjectionViewModel;
uniform float time;

void main() {

	float pulse = sin (time) * 0.5f + 0.5f;
	vec4 offset = vec4(pulse, pulse, pulse, 1);
	gl_Position = ProjectionViewModel * (Position + offset);
}