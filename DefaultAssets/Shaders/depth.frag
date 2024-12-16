#version 330 core

in vec3 screenPos;
in vec3 fragPos;

//OPTIONAL
uniform bool avoid_enabled;
uniform mat4 avoid_matrix;

//OUT
out vec4 FragColor;

void main(){
	float out_c = gl_FragCoord.z;

	vec4 avoid_position = avoid_matrix * vec4(fragPos, 1.0);
	vec3 avoid_position_norm = avoid_position.xyz / avoid_position.w;
	avoid_position_norm = avoid_position_norm * 0.5 + 0.5;

	avoid_position_norm = step(0.47, abs(avoid_position_norm - 0.5));
	float outside = max(avoid_position_norm.x, avoid_position_norm.y);

	float avoid_out_c = out_c * outside;
	avoid_out_c += 1 - outside;

	out_c = mix(out_c, avoid_out_c, float(avoid_enabled));

	FragColor = vec4(1, 1, 1, 1.0f) * out_c;
}