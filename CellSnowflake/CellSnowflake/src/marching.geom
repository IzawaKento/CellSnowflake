#version 460 core

flat in vec4 vColor[];
in mat4 vModelview[];
in mat4 vProjection[];

flat out vec4 gColor;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
void main(){
	for(int i = 0; i < gl_in.length(); ++i){
		gColor = vColor[i];
		if(vColor[i].a > 0){
			gl_Position = vProjection[i] * vModelview[i] * gl_in[i].gl_Position;
			EmitVertex();
		}
	}
	EndPrimitive();
}