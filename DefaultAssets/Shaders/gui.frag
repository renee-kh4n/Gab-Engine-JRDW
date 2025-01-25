#version 330 core

//INPUTS
in vec2 texCoord;

//OPTIONAL
uniform vec4 color;
uniform bool has_sprite;
uniform sampler2D sprite;

//OUT
out vec4 FragColor;

void main(){
	vec4 final_color = color;

	if(has_sprite)
		final_color *= texture(sprite, texCoord);

	FragColor = final_color;
}