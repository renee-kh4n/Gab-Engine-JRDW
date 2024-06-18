#version 330 core
//full fledged point light

in vec3 pass_camPos;
in vec2 texCoord;
in vec3 normCoord;
in vec3 tanCoord;
in vec3 fragPos;

//TEXTURES
uniform sampler2D texdiffuse;
uniform bool hasNormalTex;
uniform sampler2D texnormal;
//COLORS
uniform vec3 color;

//SKYBOX AMBIENT LIGHT
uniform samplerCube skybox;

//DIRECTIONAL LIGHT
struct DirLight
{
	vec3 dir;
	vec3 color;
}dirlight;

//POINT LIGHT
struct PointLight
{
	vec3 pos;
	vec3 color;
};

//CONE LIGHT
struct ConeLight
{
	vec3 pos;
	vec3 dir;
	float outer_angle;
	float inner_angle;
	vec3 color;
};

const int pointlight_count = 10;
const int conelight_count = 10;

layout (std140) uniform LightBlock{
	PointLight pointlights[pointlight_count];
	ConeLight conelights[conelight_count];
};

//AMBIENT LIGHT
uniform float ambientLightIntensity;

//SPEC LIGHTING
uniform float specStrength;
uniform float specPhong;

//OUT
out vec4 FragColor;

void main(){
	//Directional Vectors
	vec3 normalBasis = vec3(0,0,1);
	if(hasNormalTex){
		vec3 mapnormal = 2.0f * texture(texnormal, texCoord).rgb - 1.0f;
		normalBasis = normalize(mapnormal);
	}

	mat3 TBN = mat3(tanCoord, normalize(cross(normCoord, tanCoord)), normCoord);
	vec3 normal = TBN * normalBasis;

	vec3 tangent = normalize(tanCoord);
	vec3 viewDir = normalize(pass_camPos - fragPos);

	vec3 light_total = vec3(0,0,0);

	//Directional light set-up
	vec3 dirLightDir = normalize(dirlight.dir);
	vec3 dDiffuse = max(dot(normal, -dirLightDir), 0.0f) * dirlight.color;
	//Spec lighting calculation
	vec3 dReflectDir = reflect(-dirLightDir, normal);
	float dSpec = pow(max(dot(dReflectDir, viewDir), 0.01), specPhong);
	vec3 dSpecColor = dSpec * specStrength * dDiffuse;
	light_total += dDiffuse + dSpecColor;

	//Point light set-up + intensity calculation from distance
	for(int i = 0; i < pointlight_count; i++){
		PointLight curlight = pointlights[i];

		vec3 offset = curlight.pos - fragPos;
		vec3 dir = normalize(offset);
		float intensity = 1 / (1 + length(offset) + (pow(offset.x, 2) + pow(offset.y, 2)));
		vec3 finalcolor = curlight.color * intensity;
		vec3 diffuse = max(dot(normal, dir) * finalcolor, 0.0f);
		light_total += diffuse;
	}

	for(int i = 0; i < conelight_count; i++){
		ConeLight curlight = conelights[i];

		vec3 cLightOffset = curlight.pos - fragPos;
		vec3 cLightDir = normalize(cLightOffset);
		float cLightIntensity = 1 / (1 + length(cLightOffset) + (pow(cLightOffset.x, 2) + pow(cLightOffset.y, 2)));
		vec3 cLightFinalColor = curlight.color * cLightIntensity;
		float cTheta     = acos(dot(cLightDir, normalize(-curlight.dir)));
		float cEpsilon   = curlight.inner_angle - curlight.outer_angle;
		float cIntensity = smoothstep(0.0, 1.0, (cTheta - curlight.outer_angle) / cEpsilon);
		vec3 cDiffuse = max(dot(normal, cLightDir) * cLightFinalColor * cIntensity, 0.0f);

		//Spec lighting calculation
		vec3 cReflectDir = reflect(-cLightDir, normal);
		float cSpec = pow(max(dot(cReflectDir, viewDir), 0.01), specPhong);
		vec3 cSpecColor = cSpec * specStrength * cDiffuse;
		light_total += cDiffuse + cSpecColor;
	}

	//Ambient calculation
	float Pi = 6.28318530718;
	vec3 ambientReflectDir = reflect(-viewDir, normal);
    float ambientReflectionLayersApplied = 0;
    float ambientBlurDirs = 6;
    float ambientBlurQual = 2;
    vec4 ambientOutput = texture(skybox, ambientReflectDir);
	vec3 ambientReflectRight = cross(ambientReflectDir, normal);
	vec3 ambientReflectUp = cross(ambientReflectRight, ambientReflectDir);

	mat3 ambientRedirMatrix = mat3(ambientReflectRight, ambientReflectUp , ambientReflectDir);

    for(float d = 0.0; d < Pi; d += Pi/ambientBlurDirs)
    {
		for(float i = 1.0/ambientBlurQual; i <= 1.0; i += 1.0/ambientBlurQual)
        {
            vec3 from_uv = ambientReflectDir + (ambientRedirMatrix * vec3(cos(d),sin(d), 1) * (1 / specPhong) * i);
			ambientReflectionLayersApplied += 1;
			ambientOutput += texture(skybox, from_uv);
        }
    }
    
    // Output to screen
    ambientOutput /= ambientReflectionLayersApplied + 1;
	vec3 ambient = ambientOutput.xyz * ambientLightIntensity;
	

	//Combine all light sources
	//FragColor = vec4(light_total + ambient, 1.0f) * max(texture(texdiffuse, texCoord), 0.2) * vec4(color, 1.0);
	FragColor = vec4(color, 1.0);
	//FragColor = vec4(normal, 1.0f);
}