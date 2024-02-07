#version 330 core

in vec2 texCoord;

uniform sampler2D tex0;

out vec4 FragColor;

void main(){
	FragColor = vec4(1, 1, 1, 1.0f) * texture(tex0, texCoord);
}