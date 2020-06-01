#version 460 core

layout(location = 0) uniform mat4 modelview;
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform vec4 litpos;
layout(location = 3) uniform vec3 litamb;
layout(location = 4) uniform vec3 litdiff;
layout(location = 5) uniform vec3 litspec;
vec4 litpos1 = { 0.0f, 0.0f, 3.5f, 1.0f };
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
	vec3 L = normalize((litpos * P.w - P * litpos.w).xyz);	//’Ê•ª‚µ‚Ä‚é
	vec3 Iamb = Kamb * litamb;
	vec3 Idiff = max(dot(color.xyz, L), 0.0) * Kdiff * litdiff + Iamb;

	vColor = color;
	vModelview = modelview;
	vProjection = projection;
	vIdiff = vec4(Idiff, 1.0);
	gl_Position = position;
}