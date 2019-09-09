#version 430 core
//uniform vec2 size;
//uniform float scale;
uniform mat4 modelview;
uniform mat4 projection;
in vec4 position;
in vec4 color;
out vec4 vertex_color;
void main()
{
	vertex_color = color;
	gl_Position = projection * modelview * position;
	
	//gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position;
}