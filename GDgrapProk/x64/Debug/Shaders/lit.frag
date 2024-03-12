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

//SKYBOX AMBIENT LIGHT
uniform samplerCube skybox;

//DIRECTIONAL LIGHT
uniform vec3 dLightDir;
uniform vec3 dLightColor;

//POINT LIGHT
uniform vec3 pLightPos;
//COLOR defines point light BRIGHTNESS
uniform vec3 pLightColor;

//CONE LIGHT
uniform vec3 cLightPos;
uniform vec3 cLightFaceDir;
uniform vec2 cLightAngle;
//COLOR defines point light BRIGHTNESS
uniform vec3 cLightColor;


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

	//Directional light set-up
	vec3 dirLightDir = normalize(dLightDir);
	vec3 dDiffuse = max(dot(normal, -dirLightDir), 0.0f) * dLightColor;
	//Spec lighting calculation
	vec3 dReflectDir = reflect(-dirLightDir, normal);
	float dSpec = pow(max(dot(dReflectDir, viewDir), 0.01), specPhong);
	vec3 dSpecColor = dSpec * specStrength * dDiffuse;
	vec3 dLightTotal = dDiffuse + dSpecColor;

	//Point light set-up + intensity calculation from distance
	vec3 pLightOffset = pLightPos - fragPos;
	vec3 pLightDir = normalize(pLightOffset);
	float pLightIntensity = 1 / (1 + length(pLightOffset) + (pow(pLightOffset.x, 2) + pow(pLightOffset.y, 2)));
	vec3 pLightFinalColor = pLightColor * pLightIntensity;
	vec3 pDiffuse = max(dot(normal, pLightDir) * pLightFinalColor, 0.0f);
	vec3 pLightTotal = pDiffuse;

	//Cone Light
	vec3 cLightOffset = cLightPos - fragPos;
	vec3 cLightDir = normalize(cLightOffset);
	float cLightIntensity = 1 / (1 + length(cLightOffset) + (pow(cLightOffset.x, 2) + pow(cLightOffset.y, 2)));
	vec3 cLightFinalColor = cLightColor * cLightIntensity;
	float cTheta     = acos(dot(cLightDir, normalize(-cLightFaceDir)));
	float cEpsilon   = cLightAngle.x - cLightAngle.y;
	float cIntensity = smoothstep(0.0, 1.0, (cTheta - cLightAngle.y) / cEpsilon);
	vec3 cDiffuse = max(dot(normal, cLightDir) * cLightFinalColor * cIntensity, 0.0f);
	//Spec lighting calculation
	vec3 cReflectDir = reflect(-cLightDir, normal);
	float cSpec = pow(max(dot(cReflectDir, viewDir), 0.01), specPhong);
	vec3 cSpecColor = cSpec * specStrength * cDiffuse;
	vec3 cLightTotal = cDiffuse + cSpecColor;

	//Diffuse calculation
	vec3 totalDiffuse = dLightTotal + pLightTotal + cLightTotal;

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
	FragColor = vec4(totalDiffuse + ambient, 1.0f) * max(texture(texdiffuse, texCoord), 0.2);
	//FragColor = vec4(normal, 1.0f);
}