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

void main(){
	vec2 offset_normalizer = viewport_size * 0.5f;

	vec2 final_min = bl_pivot + (bl_offset / offset_normalizer);
	vec2 final_max = tr_pivot + (tr_offset / offset_normalizer);
	float x = mix(final_min.x, final_max.x, smoothstep(aPos.x, -1, 1));
	float y = mix(final_min.y, final_max.y, smoothstep(aPos.y, -1, 1));


	gl_Position = transform * vec4(aPos, 1.0);
}
