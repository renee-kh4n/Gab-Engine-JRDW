#version 330 core
//full fledged point light

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

uniform sampler2D tex0;

//POINT LIGHT
uniform vec3 pLightPos;
//COLOR defines point light BRIGHTNESS
uniform vec3 pLightColor;

//AMBIENT LIGHT
uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;

//CAMERA
uniform vec3 cameraPos;

//SPEC LIGHTING
uniform float specStrength;
uniform float specPhong;

//OUT
out vec4 FragColor;

void main(){
	//Directional Vectors
	vec3 normal = normalize(normCoord);
	vec3 viewDir = normalize(cameraPos - fragPos);

	//Point light set-up + intensity calculation from distance
	vec3 pLightOffset = pLightPos - fragPos;
	vec3 pLightDir = normalize(pLightOffset);
	float pLightIntensity = 1 / (pow(pLightOffset.x, 2) + pow(pLightOffset.y, 2));
	vec3 pLightFinalColor = pLightColor * pLightIntensity;

	//Diffuse calculation
	float diff = max(dot(normal, pLightDir), 0.0f);
	vec3 diffuse = diff * pLightFinalColor;

	//Ambient calculation
	vec3 ambient = ambientLightColor * ambientLightIntensity;

	//Spec lighting calculation
	vec3 reflectDir = reflect(-pLightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), specPhong);
	vec3 specColor = spec * specStrength * pLightFinalColor;

	//Combine all light sources
	FragColor = vec4(diffuse + ambient + specColor, 1.0f) * texture(tex0, texCoord);
}