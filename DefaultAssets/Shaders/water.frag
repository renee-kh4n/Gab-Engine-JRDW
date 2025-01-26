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
uniform float total_time;
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
uniform vec3 ambientLightTint;

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

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
	float unit = 1.0f/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(3.14*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res){
	float persistance = .5;
	float n = 0.;
	float normK = 0.;
	float f = 4.;
	float amp = 1.;
	int iCount = 0;
	for (int i = 0; i<50; i++){
		n+=amp*noise(p, f);
		f*=2.;
		normK+=amp;
		amp*=persistance;
		if (iCount == res) break;
		iCount++;
	}
	float nf = n/normK;
	return nf*nf*nf*nf;
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

		if(current_group != curlight.group_id){
			light_total += group_total / float(group_increment);
			group_total = vec3(0,0,0);
			current_group = curlight.group_id;
			group_increment = 0;
		}
		group_increment++;
	}

	light_total += group_total;
	light_total = round(light_total * 6.0f) / 6.0f;

	//Combine all light sources
	vec4 final_color = color * (pNoise(texCoord * 10, 2) + 1.0f);

	final_color = vec4(max(light_total, 0.1f) + ambientLightTint, 1.0f) * final_color;

	FragColor = final_color;
}