#version 330 core

//OPTIONAL
uniform vec4 color;
uniform sampler2D sprite;

//OUT
out vec4 FragColor;

void main(){
	FragColor = color;
}