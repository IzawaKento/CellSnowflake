#version 430 core
in vec4 gColor;
out vec4 fragment;
void main()
{
	fragment = gColor;
	//fragment = vec4(1.0, 1.0, 1.0, 1.0);
}