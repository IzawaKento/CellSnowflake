#version 460 core
//uniform vec2 size;
//uniform float scale;
uniform mat4 modelview;
uniform mat4 projection;

in vec4 position;
in vec4 color;
in uint flags;
in vec3 neighbourSurfaceDir;
in vec3 vertexNormal;

out vec4 vColor;
out uint vFlags;
out vec3 vNeighSurDir;
out mat4 vModelview;
out mat4 vProjection;

const vec3 Kamb = vec3(0.6, 0.6, 0.2);
const vec3 Kdiff = vec3(0.6, 0.6, 0.2);
const vec3 Kspec = vec3(0.3, 0.3, 0.3);
const float Kshi = 30.0;

void main()
{
	vColor = color;
	vFlags = flags;
	vNeighSurDir = neighbourSurfaceDir;
	vModelview = modelview;
	vProjection = projection;
	
	gl_Position = position;//projection * modelview * position;
	
	//gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position;
}