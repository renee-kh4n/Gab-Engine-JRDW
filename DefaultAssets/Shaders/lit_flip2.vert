#version 330 core

//CONSTS
const int max_light_count = 10;

//GEOMETRY INFORMATION
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTan;

//TRANSFORMATIONS
uniform mat4 transform_model;
uniform mat4 transform_projection;

//LIGHT
uniform mat4 light_view_matrixes[max_light_count];

//PASS TO PIXEL SHADER
out mat4 pass_transmat;
out vec2 texCoord;
out vec3 normCoord;
out vec3 tanCoord;
out vec3 fragPos;
out vec3 light_space_positions[max_light_count];

void main(){
	for(int i = 0; i < max_light_count; i++){
		vec4 initial_lightspacepoint = light_view_matrixes[i] * transform_model * vec4(aPos, 1.0);
		vec3 lightspacepos = initial_lightspacepoint.xyz / initial_lightspacepoint.w;
		lightspacepos = lightspacepos * 0.5 + 0.5;
		light_space_positions[i] = lightspacepos;
	}
	
	pass_transmat = transform_projection;
	vec4 projection_pos = pass_transmat * transform_model * vec4(aPos, 1.0);
	gl_Position = projection_pos;
	
	texCoord = aTex;

	fragPos = vec3(transform_model * vec4(aPos, 1.0));
	normCoord = -normalize(vec3(transform_model * vec4(aNormal, 0.0)));
	tanCoord = -normalize(vec3(transform_model * vec4(aTan, 0.0)));
}
