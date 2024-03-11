#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float saturation;
uniform float blur;
uniform vec4 tint;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, p - K.xxx, c.y);
}

void main()
{
    vec4 color = texture(screenTexture, TexCoords);

    //BLUR EDITING
    float Pi = 6.28318530718;

    float gblurDirs = 16.0;
    float gblurQual = 3.0;
    vec4 gblurOutput = color;

    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/gblurDirs)
    {
		for(float i=1.0/gblurQual; i<=1.0; i+=1.0/gblurQual)
        {
			gblurOutput += texture(screenTexture, TexCoords + vec2(cos(d),sin(d)) * blur * i);		
        }
    }
    
    // Output to screen
    gblurOutput /= gblurQual * gblurDirs - 15.0;
    color = gblurOutput;

    //SATURATION EDITING
    vec3 hsv = rgb2hsv(color.rgb).xyz;
    hsv.y *= saturation;
    color = vec4(hsv2rgb(hsv), color.w) * tint;

    FragColor = color;
}