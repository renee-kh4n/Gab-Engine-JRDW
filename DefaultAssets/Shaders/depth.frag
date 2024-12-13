#version 330 core

in vec3 screenPos;

//OUT
out vec4 FragColor;

void main(){
	float out_c = gl_FragCoord.z;

	FragColor = vec4(out_c, out_c, out_c, 1.0f);
}