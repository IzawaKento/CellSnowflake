#version 430 core
in vec4 vertex_color;
out vec4 fragment;
void main()
{
	fragment = vertex_color;
	//fragment = vec4(1.0, 1.0, 1.0, 1.0);
}