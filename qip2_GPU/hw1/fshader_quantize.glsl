#version 120

varying	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader
uniform	sampler2D u_Sampler;	// uniform variable for the texture image
uniform float     u_Levels;	// level value
uniform int       u_Dither;	// dither flag value

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14159);
    return  fract(sin(sn) * c);
}

void main() {

	highp	float scale = 1.0 / u_Levels;
	highp	float bias  = scale / 2;
	vec3	clr = texture2D(u_Sampler, v_TexCoord).rgb;
	highp	float dither = u_Dither*bias*rand(vec2(gl_FragCoord.x, gl_FragCoord.y))*2.0;

	float r = bias + scale * floor((clr.r+dither)*u_Levels);
	float g = bias + scale * floor((clr.g+dither)*u_Levels);
	float b = bias + scale * floor((clr.b+dither)*u_Levels);

	gl_FragColor = vec4(r, g, b, 1.0);

}
