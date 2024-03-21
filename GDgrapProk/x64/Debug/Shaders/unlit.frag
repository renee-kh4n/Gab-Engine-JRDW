#version 330 core
//full fledged point light

in vec3 fragPos;

uniform vec3 color;

//OUT
out vec4 FragColor;

void main(){
	FragColor = vec4(color, 1.0f);
}