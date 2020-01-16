#version 460 core

uniform mat4 modelview;
uniform mat4 projection;
in vec4 position;
in vec4 color;

flat out vec4 vColor;
out mat4 vModelview;
out mat4 vProjection;

void main(){
	vColor = color;
	vModelview = modelview;
	vProjection = projection;
	gl_Position = position;
}