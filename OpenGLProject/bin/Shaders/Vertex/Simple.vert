#version 410

layout (location = 0) in vec4 Position;

uniform mat4 ProjectionViewModel;
uniform float time;

void main() {
	gl_Position = ProjectionViewModel * Position;
}