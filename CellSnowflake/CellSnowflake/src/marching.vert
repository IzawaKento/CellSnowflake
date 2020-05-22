#version 460 core

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 litpos;
vec3 Lamb = vec3(0.2);
vec3 Ldiff = vec3(1.0);
vec3 Lspec = vec3(1.0);
in vec4 position;
in vec4 color;

flat out vec4 vColor;
out mat4 vModelview;
out mat4 vProjection;
out vec4 vIdiff;

const vec3 Kamb = vec3(1.5, 1.5, 1.5);
const vec3 Kdiff = vec3(0.6, 0.6, 0.6);
const vec3 Kspec = vec3(0.3, 0.3, 0.3);
const float Kshi = 30.0;

void main(){
	vec4 P = modelview * position;
	vec3 L = normalize((litpos * P.w - P * litpos.w).xyz);
	vec3 Iamb = Kamb * Lamb;
	vec3 Idiff = max(dot(color.xyz, L), 0.0) * Kdiff * Ldiff + Iamb;

	vColor = color;
	vModelview = modelview;
	vProjection = projection;
	vIdiff = vec4(Idiff, 1.0);
	gl_Position = position;
}