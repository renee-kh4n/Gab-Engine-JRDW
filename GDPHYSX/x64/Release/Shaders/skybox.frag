#version 330 core

in vec3 texCoord;

uniform samplerCube skybox;

out vec4 FragColor;

void main(){
	FragColor = texture(skybox, texCoord);
	//FragColor = vec4(texCoord, 1.0);
}
