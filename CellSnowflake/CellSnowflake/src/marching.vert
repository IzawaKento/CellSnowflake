#version 460 core

uniform mat4 modelview;
uniform mat4 projection;
in vec4 position;
in vec4 color;

out vec4 vColor;

void main(){
	vColor = color;
	gl_Position = projection * modelview * position;
}