#version 330 core

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

uniform sampler2D tex0;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;

uniform vec3 cameraPos;
uniform float specStrength;
uniform float specPhong;

out vec4 FragColor;

void main(){
	vec3 normal = normalize(normCoord);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	vec3 ambient = ambientLightColor * ambientLightIntensity;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStrength * lightColor;

	FragColor = vec4(diffuse + ambient + specColor, 1.0f) * texture(tex0, texCoord);
}