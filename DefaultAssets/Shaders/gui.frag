#version 330 core

//INPUTS
in vec2 texCoord;
in vec2 screenpos;

//OPTIONAL
uniform vec4 color;
uniform bool has_sprite;
uniform sampler2D sprite;
uniform sampler2D mainbackground;

//OUT
out vec4 FragColor;

void main(){
	vec4 final_color = color;

	if(has_sprite){
		final_color *= texture(sprite, texCoord);
		final_color.w = 1 - step(final_color.x + final_color.y + final_color.z, 0.1f);
		}
	final_color = mix(texture(mainbackground, screenpos.xy), final_color, final_color.a);
	FragColor = final_color;
}