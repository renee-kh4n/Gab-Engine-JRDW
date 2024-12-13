#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTan;

//CAMERA
uniform vec3 cameraPos;

//TRANSFORMATION
uniform mat4 transform_model;
uniform mat4 transform_projection;

out vec3 pass_camPos;
out vec2 texCoord;
out vec3 normCoord;
out vec3 tanCoord;
out vec3 fragPos;
out float depth;

void main(){
	gl_Position = transform_projection * transform_model * vec4(aPos, 1.0);
	
	pass_camPos = cameraPos;
	texCoord = aTex;

	fragPos = vec3(transform_model * vec4(aPos, 1.0));
	normCoord = normalize(vec3(transform_model * vec4(aNormal, 0.0)));
	tanCoord = normalize(vec3(transform_model * vec4(aTan, 0.0)));
	depth = length(fragPos - cameraPos);
}
