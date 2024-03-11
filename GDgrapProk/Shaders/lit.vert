#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTan;

uniform mat4 transform_model;
uniform mat4 transform_projection;

out vec2 texCoord;
out vec3 normCoord;
out vec3 fragPos;
out mat3 TBN;

void main(){
	gl_Position = transform_projection * vec4(aPos, 1.0);
	texCoord = aTex;

	fragPos = vec3(transform_model * vec4(aPos, 1.0));
	normCoord = vec3(transform_model * vec4(aNormal, 0.0));

	vec3 T = vec3(transform_model * vec4(aTan,   0.0));
	vec3 N = vec3(transform_model * vec4(aNormal,    0.0));
	mat3 TBN = mat3(T, cross(N, T), N);
}
