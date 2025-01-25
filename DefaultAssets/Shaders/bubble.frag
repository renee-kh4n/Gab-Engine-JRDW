#version 430 core

//CONSTS
const int max_light_count = 10;

//IMPORT FROM GEOMETRY SHADER
in mat4 pass_transmat;
in vec2 texCoord;
in vec3 normCoord;
in vec3 tanCoord;
in vec3 fragPos;
in vec3 light_space_positions[max_light_count];

//CAMERA
uniform vec3 cameraPos;
uniform float near_clip;
uniform float far_clip;

//TEXTURES
uniform bool hasDiffuseTex = false;
uniform sampler2D texdiffuse;
uniform sampler2D texoverlaying;
uniform sampler2D scenedepth;
uniform bool hasNormalTex = false;
uniform sampler2D texnormal;
uniform samplerCube skybox;
//SHADING
uniform vec4 color;
uniform bool transparency;

//LIGHT
uniform float ambientLightIntensity;

const uint DIRECTIONAL_LIGHT = uint(0);
const uint POINT_LIGHT = uint(1);
const uint CONE_LIGHT = uint(2);

layout(location = 32) uniform struct Light
{
	bool enabled;
	int group_id;
	int type;



	//GENERIC
	vec3 pos;
	vec3 color;

	//DIRECTIONAL
	vec3 dir;

	//NON DIRECTIONAL
	float radius;

	//CONE
	float outer_angle;
	float inner_angle;
}lights[max_light_count];

uniform sampler2D lightviews[max_light_count];

uniform float specStrength;
uniform float specPhong;

//OUT
out vec4 FragColor;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, p - K.xxx, c.y);
}

void main(){

	//Directional Vectors
	vec4 clip_space = pass_transmat * vec4(fragPos, 1.0f);
	vec3 screenPos = clip_space.xyz / clip_space.w;
	screenPos = screenPos * 0.5 + 0.5;
	float linear_depth = near_clip * far_clip / (far_clip + screenPos.z * (near_clip - far_clip));
	linear_depth = smoothstep(near_clip, far_clip, linear_depth);

	vec3 normalBasis = vec3(0,0,1);
	if(hasNormalTex){
		vec3 mapnormal = 2.0f * texture(texnormal, texCoord).rgb - 1.0f;
		normalBasis = normalize(mapnormal);
	}

	mat3 TBN = mat3(tanCoord, normalize(cross(normCoord, tanCoord)), normCoord);
	vec3 normal = TBN * normalBasis;

	vec3 tangent = normalize(tanCoord);
	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 light_total = vec3(0,0,0);
	vec3 group_total = vec3(0,0,0);
	int group_increment = 0;
	int current_group = 0;

	for(int i = 0; i < max_light_count; i++)
	{
		Light curlight = lights[i];

		if(curlight.enabled == false){
			continue;
		}

		vec3 light_space_pos = light_space_positions[i];
		
		if(curlight.type == DIRECTIONAL_LIGHT)
		{
			vec3 dir = normalize(curlight.dir);

			if(group_increment == 0){
				float diffuse = max(dot(normal, -dir), 0.0f);
				//Spec lighting calculation
				vec3 reflectdir = reflect(dir, normal);
				float spec = pow(max(dot(viewDir, reflectdir), 0.0), specPhong) * specStrength;
				group_total = (diffuse + spec) * curlight.color;
			}

			//Shadow calculation
			float totalshadow = 0.0;
			vec2 texelSize = 1.0 / textureSize(lightviews[i], 0);
			float bias = 0.002;
			for(int x = -1; x <= 1; ++x)
			{
				for(int y = -1; y <= 1; ++y)
				{
					float dSpaceNearestDepth = texture(lightviews[i], light_space_pos.xy + vec2(x, y) * texelSize).r; 
					totalshadow += step(light_space_pos.z, dSpaceNearestDepth + bias);
				}    
			}
			totalshadow /= 9.0;

			group_total *= totalshadow;
		}
		/*
		else if(curlight.type == POINT_LIGHT){
			vec3 offset = curlight.pos - fragPos;
			vec3 dir = normalize(offset);
			float intensity = 1 / (1 + length(offset) + (pow(offset.x, 2) + pow(offset.y, 2)));
			vec3 finalcolor = curlight.color * intensity;
			vec3 diffuse = max(dot(normal, dir) * finalcolor, 0.0f);
			light_total += diffuse;
		}
		else if(curlight.type == CONE_LIGHT){
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
		*/

		if(current_group != curlight.group_id){
			light_total += group_total / float(group_increment);
			group_total = vec3(0,0,0);
			current_group = curlight.group_id;
			group_increment = 0;
		}
		group_increment++;
	}

	light_total += group_total;

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
	vec4 final_color = vec4(light_total, 1.0f) * color;

	if(hasDiffuseTex){
		final_color = final_color * texture(texdiffuse, texCoord);
	}
	
	float freshnel = pow(max(1 - dot(viewDir, normal), 0), 2) * 1;
	final_color = max(final_color, freshnel);
	final_color = max(final_color, 0);

	vec4 transparency_layer = texture(texoverlaying, vec2(screenPos));

	if(transparency){
		float final_depth_here = texture(scenedepth, vec2(screenPos)).r;

		final_color += transparency_layer * step(gl_FragCoord.z, final_depth_here + 0.001f);
	}
	FragColor = final_color;
}