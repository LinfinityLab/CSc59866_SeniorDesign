#version 120

varying	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader

uniform int       u_Wsize;	// blur width value
uniform float	  u_Step;
uniform	sampler2D u_Sampler;	// uniform variable for the texture image


void main() 
{
	vec3 avg = vec3(0.0);
	vec2 tc  = v_TexCoord;
	int  w2  = u_Wsize / 2;
	for(int i=-w2; i<=w2; ++i)
		avg += texture2D(u_Sampler, vec2(tc.x + i*u_Step, tc.y)).rgb;
	avg = avg / (u_Wsize);
	gl_FragColor = vec4(avg, 1.0);
}