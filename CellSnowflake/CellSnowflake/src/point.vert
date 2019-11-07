#version 430 core
//uniform vec2 size;
//uniform float scale;
uniform mat4 modelview;
uniform mat4 projection;
in vec4 position;
in vec4 color;
in uint flags;
out vec4 vColor;
out uint vFlags;
void main()
{
	vColor = color;
	vFlags = flags;
	gl_Position = projection * modelview * position;
	
	//gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position;
}