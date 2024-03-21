#version 330 core
//full fledged point light

in float depth;

//OUT
out vec4 FragColor;

void main(){
	FragColor = vec4(depth, depth, depth, 1.0f);
}