#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

//TRANSFORMATIONS
uniform mat4 transform;

uniform vec2 viewport_size;

uniform vec2 bl_pivot;
uniform vec2 tr_pivot;
uniform vec2 bl_offset;
uniform vec2 tr_offset;

//PASS TO PIXEL SHADER
out vec2 texCoord;
out vec2 screenpos;

void main(){
	screenpos = (transform * vec4(aPos, 1.0)).xy * .5f - 0.5f;
	vec2 offset_normalizer = viewport_size * 0.5f;

	texCoord = aTex;
	gl_Position = transform * vec4(aPos, 1.0);
}
