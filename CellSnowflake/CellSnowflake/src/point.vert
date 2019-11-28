#version 430 core
//uniform vec2 size;
//uniform float scale;
uniform mat4 modelview;
uniform mat4 projection;

uniform vec4 Lpos = vec4(0.0, 0.0, 5.0, 1.0);
uniform vec3 Lamb = vec3(0.2);
uniform vec3 Ldiff = vec3(1.0);
uniform vec3 Lspec = vec3(1.0);

in vec4 position;
in vec4 color;
in uint flags;
in vec3 neighbourSurfaceDir;
in vec3 vertexNormal;

out vec4 vColor;
out uint vFlags;
out vec3 vNeighSurDir;
out vec3 vIdiff;
out vec3 vIspec;
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

	//‰A‰e‚Â‚¯
	vec4 P = modelview * position;
	vec3 L = normalize((Lpos * P.w - P * Lpos.w).xyz);
	vec3 Iamb = Kamb * Lamb;
	vIdiff = max(dot(vertexNormal, L), 0.0) * Kdiff * Ldiff + Iamb;

	vec3 V = -normalize(P.xyz);
	vec3 R = reflect(-L, vertexNormal);
	vIspec = pow(max(dot(R, V), 0.0), Kshi) * Kspec * Lspec;
	
	gl_Position = position;//projection * modelview * position;
	
	//gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position;
}