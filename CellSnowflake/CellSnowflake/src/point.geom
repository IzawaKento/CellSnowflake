#version 430 core
layout (points) in;
layout (points) out;
void main()
{
	gl_Position = gl_in[0].gl_Position;
	/*
	if(vertex_color.a > 0.0){
		EmitVertex();
	}
	*/
	EmitVertex();
	EndPrimitive();
}